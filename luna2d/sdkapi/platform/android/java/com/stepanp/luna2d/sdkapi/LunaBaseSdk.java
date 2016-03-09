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

package com.stepanp.luna2d.sdkapi;

import android.app.Activity;
import com.stepanp.luna2d.LunaActivity;
import com.stepanp.luna2d.LunaGlView;

public abstract class LunaBaseSdk
{
	// Check for value with given name exists in config
	public native boolean hasConfigValue(String name);

	// Get string value  from config by string
	public native String getConfigString(String name);

	// Get int value from config by string
	public native int getConfigInt(String name);

	// Get float value from config by string
	public native float getConfigFloat(String name);

	// Get bool value from config by string
	public native boolean getConfigBool(String name);

	// Get main game activity
	// Some android APIs requests Context, or Activity for work.
	// So, use Activity returning this method
	public Activity getSharedActivity()
	{
		return LunaActivity.getSharedActivity();
	}

	// Run given runnable in UI thread
	// Most SDK module functions calls from game thread
	// So, all actions with UI should be wrapped with this method
	public void runInUiThread(Runnable runnable)
	{
		getSharedActivity().runOnUiThread(runnable);
	}

	// Run given runnable in game thread
	// Game run in separate thread.
	// So all actions from UI should be wrapped with this method
	public void runInRenderThread(Runnable runnable)
	{
		LunaGlView.getSharedGlView().queueEvent(runnable);
	}
}
