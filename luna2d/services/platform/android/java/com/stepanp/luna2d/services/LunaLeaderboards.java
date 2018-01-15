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
import android.content.IntentSender;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;
import com.google.android.gms.common.ConnectionResult;
import com.stepanp.luna2d.services.api.LunaServicesApi;
import com.stepanp.luna2d.services.api.LunaActivityListener;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.games.Games;

public class LunaLeaderboards
{
	private static final int RC_SIGN_IN = 9001;
	private static final int RC_UNUSED = 5001;

	public static void init()
	{
		leaderboardId = LunaServicesApi.getConfigString("leaderboardId");
		if(leaderboardId.isEmpty()) return;

		apiClient = new GoogleApiClient.Builder(LunaServicesApi.getSharedActivity())
				.addOnConnectionFailedListener(connectionFailedListener)
				.addConnectionCallbacks(connectionListener)
				.addApi(Games.API).addScope(Games.SCOPE_GAMES)
				.build();

		if(apiClient == null) return;

		LunaServicesApi.addActivityListener(activityListener);
		apiClient.connect();
	}

	private static GoogleApiClient apiClient;
	private static String leaderboardId;
	private static boolean resolvingConnectionFailure = false;
	private static boolean notConnected = false;

	private static boolean isSigned()
	{
		return apiClient != null && apiClient.isConnected();
	}

	// Submit score to leadearboard
	public static void submitScore(int score)
	{
		if(leaderboardId.isEmpty()) Log.e(LunaServicesApi.getLogTag(), "\"leaderboardId\" must be set in config to submit score");
		if(!isSigned()) return;
		
		Games.Leaderboards.submitScore(LunaLeaderboards.apiClient, leaderboardId, score);
	}

	// Open leadearboards popup
	public static void open()
	{
		if(apiClient == null) return;

		if(!isSigned())
		{
			apiClient.connect();
			return;
		}

		Activity activity = LunaServicesApi.getSharedActivity();
		activity.startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(apiClient), RC_UNUSED);
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

	private static OnConnectionFailedListener connectionFailedListener = new OnConnectionFailedListener()
	{
		@Override
		public void onConnectionFailed(@NonNull ConnectionResult connectionResult)
		{
			if(resolvingConnectionFailure) return;

			resolvingConnectionFailure = true;
			if(!resolveConnectionFailure(connectionResult))
			{
				resolvingConnectionFailure = false;
				notConnected = true;
			}
		}

        private boolean resolveConnectionFailure(ConnectionResult result)
        {
            if(!result.hasResolution()) return false;

            try
            {
                result.startResolutionForResult(LunaServicesApi.getSharedActivity(), RC_SIGN_IN);
                return true;
            }
            catch (IntentSender.SendIntentException e)
            {
                apiClient.connect();
                return false;
            }
        }
	};

	private static LunaActivityListener activityListener = new LunaActivityListener()
	{
		@Override
		public void onStart(Activity activity)
		{
			if(apiClient == null || notConnected) return;
			if(!isSigned()) apiClient.connect();
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

		@Override
		public boolean onActivityResult(int requestCode, int resultCode, Intent intent)
		{
			if (requestCode == RC_SIGN_IN)
			{
				resolvingConnectionFailure = false;
				if(resultCode == Activity.RESULT_OK) apiClient.connect();
				else notConnected = true;
			}

			return false;
		}
	};
}
