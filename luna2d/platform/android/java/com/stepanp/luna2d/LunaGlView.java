//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

package com.stepanp.luna2d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.view.MotionEvent;

public class LunaGlView extends GLSurfaceView
{	
	private enum TouchType
	{
		DOWN,
		MOVED,
		UP,
	}
	
	private static class TouchEvent implements Runnable
	{
		public TouchEvent(TouchType type, float x, float y, int touchIndex)
		{
			this.x = x;
			this.y = y;
			this.type = type;
			this.touchIndex = touchIndex;
		}
		
		public TouchType type;
		public float x, y;
		public int touchIndex;
		
		@Override
		public void run()
		{
			switch(type)
			{
			case DOWN:
				LunaNative.onTouchDown(x, y, touchIndex);
				break;
			case MOVED:
				LunaNative.onTouchMoved(x, y, touchIndex);
				break;
			case UP:
				LunaNative.onTouchUp(x, y, touchIndex);
				break;
			}
		}
	}
	
	public LunaGlView(Activity activity)
	{
		super(activity);
		
		setEGLContextClientVersion(2); // Enable OpenGL ES 2.0
		renderer = new GlRenderer(activity);
		setRenderer(renderer);
	}
	
	private GlRenderer renderer;
	
	// Handle touch event in renderer thread
	private void queueTouchEvent(TouchType type, float x, float y, int touchIndex)
	{
		queueEvent(new TouchEvent(type, x, y, touchIndex));
	}
	
	// Handling touch events
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int action = event.getActionMasked();
		int pointerIndex = event.getActionIndex();
		int pointerId = event.getPointerId(pointerIndex);
		int pointersCount =  event.getPointerCount();
		
		// Invert Y-axis,
		// because OpenGl origin in bottom of screen,
		// but Android View origin in top of screen				
		float y = getHeight() - event.getY(pointerIndex);
		
		switch(action)
		{
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
			queueTouchEvent(TouchType.DOWN, event.getX(pointerIndex), y, pointerId);
			break;
			
		case MotionEvent.ACTION_MOVE:
			for(int i = 0; i < pointersCount; i++) 
			{
				float pointerY = getHeight() - event.getY(i);
				queueTouchEvent(TouchType.MOVED, event.getX(i), pointerY, event.getPointerId(i));
			}
			break;
			
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
		case MotionEvent.ACTION_OUTSIDE:
			queueTouchEvent(TouchType.UP, event.getX(pointerIndex), y, pointerId);
			break;
			
		// This event triggered when gesture was canceled (E.g. when phone change screen orientation)
		// Call "onTouchUp" for all pointers in that case
		case MotionEvent.ACTION_CANCEL:
			for(int i = 0; i < pointersCount; i++) 
			{
				float pointerY = getHeight() - event.getY(i);
				queueTouchEvent(TouchType.UP, event.getX(i), pointerY, event.getPointerId(i));
			}
			
			break;
		}
		
		return true;
	}
	
	@Override
	protected void onSizeChanged(int newWidth, int newHeight, int oldWidth, int oldHeight)
	{
		if(!LunaNative.isInitialized()) renderer.setScreenSize(newWidth, newHeight);
	}
	
	
	private static class GlRenderer implements GLSurfaceView.Renderer
	{
		public GlRenderer(Activity activity)
		{
			this.activity = activity;
		}
		
		private Activity activity;
		private int screenWidth;
		private int screenHeight;
		
		public void setScreenSize(int width, int height)
		{
			screenWidth = width;
			screenHeight = height;
		}
		
		@Override
		public void onSurfaceCreated(GL10 unused, EGLConfig config)
		{
			// Initialize engine at first creating of surface
			if(!LunaNative.isInitialized())
			{			
				// Get application label
				ApplicationInfo appInfo = activity.getApplicationInfo();
				String appName = (String)activity.getPackageManager().getApplicationLabel(appInfo);
				
				// Get path to this .apk file		
				String apkPath = activity.getPackageCodePath();  
				
				// Get path to application folder in "data/data/"
				String appFolderPath = activity.getFilesDir().getAbsolutePath();
				
				LunaNative.initialize(screenWidth, screenHeight, appName, apkPath, appFolderPath);
			}
			
			// When surface was rectreated, we need reload some assets: textures, shaders, etc.
			else
			{
				LunaNative.reloadAssets();
			}
		}

		@Override
		public void onSurfaceChanged(GL10 unused, int width, int height)
		{
		}

		@Override
		public void onDrawFrame(GL10 unused)
		{
			LunaNative.mainLoop();
		}
	}
}