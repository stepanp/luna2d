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
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import java.util.Calendar;
import com.stepanp.luna2d.LunaActivity;

public class LunaNotifications
{
	private static Notification makeNotification(Context context, String message)
	{
		ApplicationInfo appInfo = context.getApplicationInfo();
		String appName = (String)context.getPackageManager().getApplicationLabel(appInfo);

		Intent intent = new Intent(context, LunaActivity.class);
		intent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);

		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);

		int appIconId = appInfo.icon;
		Bitmap largeIcon = BitmapFactory.decodeResource(context.getResources(), appIconId);

		int notificationIcon = context.getResources().getIdentifier("ic_notification", "mipmap", context.getPackageName());
		if(notificationIcon == 0) notificationIcon = appIconId;

		Notification.Builder builder = new Notification.Builder(context);

		builder.setContentTitle(appName);
		builder.setContentText(message);
		builder.setTicker(message);
		builder.setSmallIcon(notificationIcon);
		builder.setLargeIcon(largeIcon);
		builder.setAutoCancel(true);
		builder.setDefaults(Notification.DEFAULT_ALL);
		builder.setContentIntent(pendingIntent);

		return builder.build();
	}

	private static PendingIntent makePendingIntent(String message, int id)
	{
		Activity activity = LunaActivity.getSharedActivity();

		Intent intent = new Intent(activity.getApplicationContext(), LunaNotifications.Receiver.class);
		intent.putExtra("id", id);
		intent.putExtra("message", message);

		return PendingIntent.getBroadcast(activity, id, intent, PendingIntent.FLAG_UPDATE_CURRENT);
	}

	// Schedule local notification
	public static void schedule(String message, int secondsFromNow, int id)
	{
		Activity activity = LunaActivity.getSharedActivity();
		PendingIntent pendingIntent = makePendingIntent(message, id);

		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(System.currentTimeMillis());
		calendar.add(Calendar.SECOND, secondsFromNow);

		AlarmManager alarmManager = (AlarmManager)activity.getSystemService(Context.ALARM_SERVICE);
		alarmManager.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pendingIntent);
	}

	// Cancel scheduled notification with specified id
	public static void cancel(int id)
	{
		Activity activity = LunaActivity.getSharedActivity();
		PendingIntent pendingIntent = makePendingIntent(null, id);

		AlarmManager alarmManager = (AlarmManager)activity.getSystemService(Context.ALARM_SERVICE);
		alarmManager.cancel(pendingIntent);

		NotificationManager manager = (NotificationManager)activity.getSystemService(Context.NOTIFICATION_SERVICE);
		manager.cancel(id);
	}

	public static class Receiver extends BroadcastReceiver
	{
		private boolean isActivityInForeground()
		{
			LunaActivity activity = (LunaActivity)LunaActivity.getSharedActivity();
			return activity != null && activity.isInForeground();
		}

		@Override
		public void onReceive(Context context, Intent intent)
		{
			if(isActivityInForeground()) return; // Suppress notifications when activity in foreground

			int id = intent.getIntExtra("id", 0);
			String message = intent.getStringExtra("message");

			Notification notification = makeNotification(context, message);
			NotificationManager manager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
			manager.notify(id, notification);
		}
	}
}
