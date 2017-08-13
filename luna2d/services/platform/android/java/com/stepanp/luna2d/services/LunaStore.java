//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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

package com.stepanp.luna2d.services;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.net.Uri;
import com.stepanp.luna2d.services.api.LunaServicesApi;
import com.sbstrm.appirater.Appirater;

public class LunaStore
{
	// Get url to page of game in store
	public static String getUrl()
	{
		String appPackage = LunaServicesApi.getSharedActivity().getPackageName();
		String url = "https://play.google.com/store/apps/details?id=" + appPackage;

		return url;
	}

	// Open page of game in store
	public static void openPage()
	{
		Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(getUrl()));
		LunaServicesApi.getSharedActivity().startActivity(intent);
	}

	// Request rate app dialog
	public static void requestRateApp()
	{
		final Activity activity = LunaServicesApi.getSharedActivity();
		int daysUntilPromt = 0;
		int launchesUntilPromt = 2;
		int daysBeforeReminding = 2;

		if(LunaServicesApi.hasConfigValue("rateAppDays")) daysUntilPromt = LunaServicesApi.getConfigInt("rateAppDays");
		if(LunaServicesApi.hasConfigValue("rateAppLaunches")) launchesUntilPromt = LunaServicesApi.getConfigInt("rateAppLaunches");
		if(LunaServicesApi.hasConfigValue("rateAppRemindingTime")) daysBeforeReminding = LunaServicesApi.getConfigInt("rateAppRemindingTime");

		ApplicationInfo appInfo = activity.getApplicationInfo();
		String appName = (String)activity.getPackageManager().getApplicationLabel(appInfo);

		Appirater.appTitle = appName;
		Appirater.daysUntilPrompt = daysUntilPromt;
		Appirater.launchesUntilPrompt = launchesUntilPromt;
		Appirater.daysBeforeReminding = daysBeforeReminding;

		LunaServicesApi.runInUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				Appirater.appLaunched(activity);
			}
		});
	}
}
