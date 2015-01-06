package com.stepanp.luna2d;

import android.app.Activity;
import android.os.Bundle;

public class LunaActivity extends Activity
{	
	protected LunaGlView glView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		// Create OpenGL surface view
		glView = new LunaGlView(getApplication());
		setContentView(glView);
	}
	
	@Override 
	protected void onPause()
	{
        super.onPause();
        glView.onPause();
    }

    @Override 
    protected void onResume()
    {
        super.onResume();
        glView.onResume();
    }
}
