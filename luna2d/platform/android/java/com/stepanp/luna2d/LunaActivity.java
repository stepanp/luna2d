//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import java.util.List;
import java.util.ArrayList;
import com.stepanp.luna2d.services.api.LunaActivityListener;

public class LunaActivity extends Activity
{
	private LunaGlView glView;
	private List<LunaActivityListener> listeners = new ArrayList<LunaActivityListener>();
	private NetworkStateReceiver networkStateReceiver;

	private static Activity sharedActivity = null;

	public static Activity getSharedActivity()
	{
		return sharedActivity;
	}

	public void addListener(LunaActivityListener listener)
	{
		listeners.add(listener);
	}

	public void removeListener(LunaActivityListener listener)
	{
		listeners.remove(listener);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		enableFullscreen();

		sharedActivity = this;
		LunaPrefs.init();
		
		// Create OpenGL surface view
		glView = new LunaGlView(this);
		setContentView(glView);

		// Create network state receiver if application has permission
		// createNetworkStateReciever();
	}
	
	@Override
	public  void onPause()
	{
		super.onPause();
		glView.onPause();

		for(LunaActivityListener listener : listeners) listener.onPause(this);
	}

	@Override
	public  void onResume()
	{
		super.onResume();
		glView.onResume();

		for(LunaActivityListener listener : listeners) listener.onResume(this);
	}

	@Override
	public  void onStart()
	{
		super.onStart();

		for(LunaActivityListener listener : listeners) listener.onStart(this);
	}

	@Override
	public  void onStop()
	{
		super.onStop();

		for(LunaActivityListener listener : listeners) listener.onStop(this);
	}

	@Override
	public  void onDestroy()
	{
		super.onDestroy();

		for(LunaActivityListener listener : listeners) listener.onDestroy(this);
	}

	@Override
	public  void onBackPressed()
	{
		boolean processEvent = true;

		for(LunaActivityListener listener : listeners)
		{
			if(listener.onBackPressed(this)) processEvent = false;
		}

		if(processEvent) super.onBackPressed();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent)
	{
		super.onActivityResult(requestCode, resultCode, intent);

		for(LunaActivityListener listener : listeners) listener.onActivityResult(requestCode, resultCode, intent);
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus)
	{
		super.onWindowFocusChanged(hasFocus);

		if(hasFocus) enableFullscreen();
	}
	
	private void enableFullscreen()
	{
		getWindow().getDecorView().setSystemUiVisibility(
			View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
			View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
			View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
			View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
			View.SYSTEM_UI_FLAG_FULLSCREEN |
			View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);	
	}

	private void createNetworkStateReciever()
	{
		networkStateReceiver = new NetworkStateReceiver();
		this.registerReceiver(networkStateReceiver, new IntentFilter(android.net.ConnectivityManager.CONNECTIVITY_ACTION));
	}

	private class NetworkStateReceiver extends BroadcastReceiver
	{
		private boolean connected;

		@Override
		public void onReceive(Context context, Intent intent)
		{
			if(intent == null || intent.getExtras() == null) return;

			ConnectivityManager manager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo netInfo = manager.getActiveNetworkInfo();

			boolean prevConnected = connected;
			connected = netInfo != null && netInfo.isAvailable();

			if(prevConnected != connected)
			{
				if(connected)
				{
					// Run listeners with delay
					// because at moment of receiving message about network becomes available
					// network can be still unavailable
					final Handler handler = new Handler();
					handler.postDelayed(new Runnable()
					{
						@Override
						public void run()
						{
							runListeners(connected);
						}
					},
					100);
				}
				else runListeners(connected);
			}
		}

		private void runListeners(boolean connected)
		{
			for(LunaActivityListener listener : listeners) listener.onNetworkStateChanged(LunaActivity.this, connected);
		}
	};
}
