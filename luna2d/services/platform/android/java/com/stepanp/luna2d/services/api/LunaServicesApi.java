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

package com.stepanp.luna2d.services.api;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import com.stepanp.luna2d.LunaActivity;
import com.stepanp.luna2d.LunaGlView;

public class LunaServicesApi
{
	private static String logTag = "";

	// Check for value with given name exists in config
	public static native boolean hasConfigValue(String name);

	// Get string value from config by name
	public static native String getConfigString(String name);

	// Get int value from config by name
	public static native int getConfigInt(String name);

	// Get float value from config by name
	public static native float getConfigFloat(String name);

	// Get bool value from config by name
	public static native boolean getConfigBool(String name);

	// Get main game activity
	// Some android APIs requests Context or Activity for work.
	// So, use Activity returning this method
	public static Activity getSharedActivity()
	{
		return LunaActivity.getSharedActivity();
	}

	// Subscribe given listener on game activity events (e.g. onPause/onResume)
	public static void addActivityListener(final LunaActivityListener listener)
	{
		final LunaActivity activity = (LunaActivity)LunaActivity.getSharedActivity();
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				activity.addListener(listener);
			}
		});
	}

	// Unsubscribe given listener from game activity events
	public static void removeActivityListener(final LunaActivityListener listener)
	{
		final LunaActivity activity = (LunaActivity)LunaActivity.getSharedActivity();
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				activity.removeListener(listener);
			}
		});
	}

	// Get string tag for logging
	public static String getLogTag()
	{
		if(logTag.isEmpty())
		{
			ApplicationInfo appInfo = getSharedActivity().getApplicationInfo();
			logTag = (String)getSharedActivity().getPackageManager().getApplicationLabel(appInfo);
		}

		return logTag;
	}

	// Run given runnable in UI thread
	// Most methods of SDK modules calls from game thread.
	// So, all actions with UI should be wrapped with this method
	public static void runInUiThread(Runnable runnable)
	{
		getSharedActivity().runOnUiThread(runnable);
	}

	// Run given runnable in game thread
	// Game running in separate thread.
	// So, all actions with game in UI thread should be wrapped with this method
	public static void runInRenderThread(Runnable runnable)
	{
		LunaGlView.getSharedGlView().queueEvent(runnable);
	}
}
