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

import java.util.Locale;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.net.Uri;
import android.util.Log;

//----------------------------------------------------------
// Java wrapper for Android implementation of platform utils
//----------------------------------------------------------
public class LunaUtils 
{
	// Get system locale
	public static String getSystemLocale()
	{
		return Locale.getDefault().toString();
	}

	// Open given URL in system browser
	public static void openUrl(String url)
	{
		Activity activity = LunaActivity.getSharedActivity();

		try
		{
			Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
			activity.startActivity(intent);
		}
		catch(ActivityNotFoundException e)
		{
			ApplicationInfo appInfo = activity.getApplicationInfo();
			String appName = (String)activity.getPackageManager().getApplicationLabel(appInfo);

			Log.e(appName, "Cannot open url: " + url);
		}
	}

	// Show native dialog with "Ok" button
	public static void messageDialog(final String title, final String message)
	{
		final Activity activity = LunaActivity.getSharedActivity();

		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(activity);
				builder.setTitle(title);
				builder.setMessage(message);

				builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						LunaGlView.getSharedGlView().queueEvent(new Runnable()
						{
							@Override
							public void run()
							{
								onMessageDialogClosed();
							}
						});
					}
				});

				builder.setOnCancelListener(new DialogInterface.OnCancelListener()
				{
					@Override
					public void onCancel(DialogInterface dialog)
					{
						LunaGlView.getSharedGlView().queueEvent(new Runnable()
						{
							@Override
							public void run()
							{
								onMessageDialogClosed();
							}
						});
					}
				});

				builder.setCancelable(true);
				builder.create().show();
			}
		});
	}

	// Show native dialog with "Yes" and "No" buttons
	public static void confirmDialog(final String title, final String message)
	{
		final Activity activity = LunaActivity.getSharedActivity();

		LunaActivity.getSharedActivity().runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(activity);
				builder.setTitle(title);
				builder.setMessage(message);

				builder.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						LunaGlView.getSharedGlView().queueEvent(new Runnable()
						{
							@Override
							public void run()
							{
								onConfirmDialogClosed(true);
							}
						});
					}
				});

				builder.setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
						LunaGlView.getSharedGlView().queueEvent(new Runnable()
						{
							@Override
							public void run()
							{
								onConfirmDialogClosed(false);
							}
						});
					}
				});

				builder.setOnCancelListener(new DialogInterface.OnCancelListener()
				{
					@Override
					public void onCancel(DialogInterface dialog)
					{
						LunaGlView.getSharedGlView().queueEvent(new Runnable()
						{
							@Override
							public void run()
							{
								onConfirmDialogClosed(false);
							}
						});
					}
				});

				builder.setCancelable(true);
				builder.create().show();
			}
		});
	}

	public static native void onMessageDialogClosed();

	public static native void onConfirmDialogClosed(boolean isConfirmed);
}
