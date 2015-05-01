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
import android.util.Log;
import android.view.MotionEvent;

public class LunaGlView extends GLSurfaceView
{	
	public LunaGlView(Activity activity)
	{
		super(activity);
		
		setEGLContextClientVersion(2); // Enable OpenGL ES 2.0
		setRenderer(new GlRenderer(activity));
	}
	
	// Handling touch events
	@Override
	public boolean onTouchEvent(final MotionEvent event)
	{
		// Handle touch events in renderer thread
		queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				int action = event.getAction() & MotionEvent.ACTION_MASK;
				int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> 
					MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				
				// Invert Y-axis,
				// because OpenGl origin in bottom of screen,
				// but Android View origin in top of screen				
				float y = getHeight() - event.getY(pointerIndex);
				
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
				case MotionEvent.ACTION_POINTER_DOWN:
					LunaNative.onTouchDown(event.getX(pointerIndex), y, event.getPointerId(pointerIndex));
					break;
					
				case MotionEvent.ACTION_MOVE:
					int count = event.getPointerCount();
					for(int i = 0; i < count; i++) 
					{
						float pointerY = getHeight() - event.getY(i);
						LunaNative.onTouchMoved(event.getX(i), pointerY, event.getPointerId(i));
					}
					break;
					
				case MotionEvent.ACTION_UP:
				case MotionEvent.ACTION_POINTER_UP:
				case MotionEvent.ACTION_OUTSIDE:
				case MotionEvent.ACTION_CANCEL:
					LunaNative.onTouchUp(event.getX(pointerIndex), y, event.getPointerId(pointerIndex));
					break;
				}
			}
		});
		
		return true;		
	}
	
	private static class GlRenderer implements GLSurfaceView.Renderer
	{
		public GlRenderer(Activity activity)
		{
			this.activity = activity;
		}
		
		private Activity activity;
		
		@Override
		public void onSurfaceCreated(GL10 unused, EGLConfig config)
		{
			// Initialize engine at first creating of surface
			if(!LunaNative.isInitialized())
			{
				// Get screen sizes
				DisplayMetrics metrics = new DisplayMetrics();
				activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
				int width = metrics.widthPixels;
				int height = metrics.heightPixels;
				
				// Get application label
				ApplicationInfo appInfo = activity.getApplicationInfo();
		        String appName = (String)activity.getPackageManager().getApplicationLabel(appInfo);
		        
		        // Get path to this .apk file		
		        String apkPath = activity.getPackageCodePath();  
		        
		        // Get path to application folder in "data/data/"
		        String appFolderPath = activity.getFilesDir().getAbsolutePath();
		        
				LunaNative.initialize(width, height, appName, apkPath, appFolderPath);
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