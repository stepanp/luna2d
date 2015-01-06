package com.stepanp.luna2d;

public class LunaNative
{
	static
	{
		System.loadLibrary("luna2d");
	}
	
	public static native void initialize(int screenWidth, int screenHeight, String appName, String apkPath);
	public static native void mainLoop();
	
	// Touch events
	public static native void onTouchDown(float x, float y);
	public static native void onTouchMoved(float x, float y);
	public static native void onTouchUp(float x, float y);
}
