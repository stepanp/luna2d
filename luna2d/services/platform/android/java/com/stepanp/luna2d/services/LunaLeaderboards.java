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

package com.stepanp.luna2d.services;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import com.stepanp.luna2d.services.api.LunaServicesApi;
import com.stepanp.luna2d.services.api.LunaActivityListener;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.games.Games;

public class LunaLeaderboards
{
	public static void init()
	{
		Activity activity = LunaServicesApi.getSharedActivity();

		apiClient = new GoogleApiClient.Builder(activity)
				.addConnectionCallbacks(connectionListener)
				.addApi(Games.API).addScope(Games.SCOPE_GAMES)
				.build();

		LunaServicesApi.addActivityListener(activityListener);
	}

	private static GoogleApiClient apiClient;

	private static boolean isSigned()
	{
		return apiClient != null && apiClient.isConnected();
	}

	// Submit score to leadearboard
	public static void submitScore(int score)
	{
		if(!isSigned()) return;
		
		Games.Leaderboards.submitScore(LunaLeaderboards.apiClient, "", score);
	}

	// Open leadearboards popup
	public static void open()
	{
		if(!isSigned()) return;

		Activity activity = LunaServicesApi.getSharedActivity();
		activity.startActivity(Games.Leaderboards.getAllLeaderboardsIntent(apiClient));
	}


	private static ConnectionCallbacks connectionListener = new ConnectionCallbacks()
	{
		@Override
		public void onConnected(@Nullable Bundle bundle)
		{

		}

		@Override
		public void onConnectionSuspended(int i)
		{
			if(apiClient != null) apiClient.connect();
		}
	};

	private static LunaActivityListener activityListener = new LunaActivityListener()
	{
		@Override
		public void onStart(Activity activity)
		{
			if(apiClient != null) apiClient.connect();
		}

		@Override
		public void onResume(Activity activity)
		{

		}

		@Override
		public void onPause(Activity activity)
		{

		}

		@Override
		public void onStop(Activity activity)
		{
			if(isSigned()) apiClient.disconnect();
		}

		@Override
		public void onDestroy(Activity activity)
		{

		}

		@Override
		public boolean onBackPressed(Activity acitivity)
		{
			return false;
		}

		@Override
		public void onNetworkStateChanged(Activity acitivity, boolean connected)
		{

		}
	};
}
