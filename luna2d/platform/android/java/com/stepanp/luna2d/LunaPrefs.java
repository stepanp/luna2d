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
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

//-------------------------------------------------
// Java wrapper for Android implementation of prefs
//-------------------------------------------------
public class LunaPrefs
{
	private final static String PREFS_NAME = "luna2d_prefs";
	
	private static SharedPreferences sharedPrefs;
	private static Editor editor;
	
	public static void init(Activity activity)
	{
		sharedPrefs = activity.getSharedPreferences(PREFS_NAME, Activity.MODE_PRIVATE);
	}
	
	private static void edit()
	{
		if(editor == null) editor = sharedPrefs.edit();
	}
	
	public static String getString(String name)
	{
		return sharedPrefs.getString(name, "");
	}
	
	public static int getInt(String name)
	{
		return sharedPrefs.getInt(name, 0);
	}
	
	public static float getFloat(String name)
	{
		return sharedPrefs.getFloat(name, 0.0f);
	}
	
	public static boolean getBool(String name)
	{
		return sharedPrefs.getBoolean(name, false);
	}
	
	public static boolean hasValue(String name)
	{
		return sharedPrefs.contains(name);
	}
	
	public static void setString(String name, String value)
	{
		edit();
		editor.putString(name, value);
		editor.commit();
	}
	
	public static void setInt(String name, int value)
	{
		edit();
		editor.putInt(name, value);
		editor.commit();
	}
	
	public static void setFloat(String name, float value)
	{
		edit();
		editor.putFloat(name, value);
		editor.commit();
	}
	
	public static void setBool(String name, boolean value)
	{
		edit();
		editor.putBoolean(name, value);
		editor.commit();
	}
	
	public static void removeValue(String name)
	{
		edit();
		editor.remove(name);
		editor.commit();
	}
	
	public static void clear()
	{
		edit();
		editor.clear();
		editor.commit();
	}
}
