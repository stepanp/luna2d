package com.stepanp.luna2d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class LunaGlView extends GLSurfaceView
{	
	public LunaGlView(Context context)
	{
		super(context);
		
		setEGLContextClientVersion(2); // Enable OpenGL ES 2.0
		setRenderer(new GlRenderer(context));
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
				int action = event.getAction();

				// Invert Y-axis,
				// because OpenGl origin in bottom of screen,
				// but Android View origin in top of screen	
				int surfaceHeight = getHeight();
				float y = surfaceHeight - event.getY();
				
				if(action == MotionEvent.ACTION_DOWN) LunaNative.onTouchDown(event.getX(), y);
				else if(action == MotionEvent.ACTION_MOVE) LunaNative.onTouchMoved(event.getX(), y);
				else if(action == MotionEvent.ACTION_UP) LunaNative.onTouchUp(event.getX(), y);
			}
		});
		
		return true;		
	}
	
	private static class GlRenderer implements GLSurfaceView.Renderer
	{
		public GlRenderer(Context context)
		{
			this.context = context;
		}
		
		private Context context;
		
		@Override
		public void onSurfaceCreated(GL10 unused, EGLConfig config)
		{
		}

		@Override
		public void onSurfaceChanged(GL10 unused, int width, int height)
		{
			// Get application label
			ApplicationInfo appInfo = context.getApplicationInfo();
	        String appName = (String)context.getPackageManager().getApplicationLabel(appInfo);
	        
	        // Get path to this .apk file
	        String apkPath = context.getPackageCodePath();
					
			LunaNative.initialize(width, height, appName, apkPath);
		}

		@Override
		public void onDrawFrame(GL10 unused)
		{
			LunaNative.mainLoop();
		}		
	}
}