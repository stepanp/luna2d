package com.sbstrm.appirater;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

/*
 * @source https://github.com/sbstrm/appirater-android
 * @license MIT/X11
 * 
 * Copyright (c) 2011-2013 sbstrm Y.K.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

public class Appirater {

	private static final String PREF_LAUNCH_COUNT = "launch_count";
	private static final String PREF_EVENT_COUNT = "event_count";
	private static final String PREF_RATE_CLICKED = "rateclicked";
	private static final String PREF_DONT_SHOW = "dontshow";
	private static final String PREF_DATE_REMINDER_PRESSED = "date_reminder_pressed";
	private static final String PREF_DATE_FIRST_LAUNCHED = "date_firstlaunch";
	private static final String PREF_APP_VERSION_CODE = "versioncode";
    private static final String PREF_APP_LOVE_CLICKED= "loveclicked";

	public static boolean testMode = false;
	public static int daysUntilPrompt = 30;
	public static int launchesUntilPrompt = 15;
	public static int eventsUntilPrompt = 15;
	public static int daysBeforeReminding = 3;
	public static String appTitle = "";
	public static String marketUrl = "market://details?id=%s";
	
    public static void appLaunched(Context mContext) {
        SharedPreferences prefs = mContext.getSharedPreferences(mContext.getPackageName()+".appirater", 0);
        if(!testMode && (prefs.getBoolean(PREF_DONT_SHOW, false) || prefs.getBoolean(PREF_RATE_CLICKED, false))) {return;}
        
        SharedPreferences.Editor editor = prefs.edit();
        
        if(testMode){
            if (prefs.getBoolean(PREF_APP_LOVE_CLICKED, true)) {
                showRateDialog(mContext,editor);
            } else {
                showLoveDialog(mContext, editor);
            }
        	return;
        }
        
        // Increment launch counter
        long launch_count = prefs.getLong(PREF_LAUNCH_COUNT, 0);

        // Get events counter
        long event_count = prefs.getLong(PREF_EVENT_COUNT, 0);

        // Get date of first launch
        long date_firstLaunch = prefs.getLong(PREF_DATE_FIRST_LAUNCHED, 0);
        
		// Get reminder date pressed
		long date_reminder_pressed = prefs.getLong(PREF_DATE_REMINDER_PRESSED, 0);
        
        try{
	        int appVersionCode = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0).versionCode;
	        if(prefs.getInt(PREF_APP_VERSION_CODE, 0)  != appVersionCode){
		        //Reset the launch and event counters to help assure users are rating based on the latest version. 
		        launch_count = 0;
		        event_count = 0;
		        editor.putLong(PREF_EVENT_COUNT, event_count);
	        }
	        editor.putInt(PREF_APP_VERSION_CODE, appVersionCode);
        }catch(Exception e){
        	//do nothing
        }
        
        launch_count++;
        editor.putLong(PREF_LAUNCH_COUNT, launch_count);

        if (date_firstLaunch == 0) {
            date_firstLaunch = System.currentTimeMillis();
            editor.putLong(PREF_DATE_FIRST_LAUNCHED, date_firstLaunch);
        }
        
        // Wait at least n days or m events before opening
        if (launch_count >= launchesUntilPrompt) {
			long millisecondsToWait = daysUntilPrompt * 24 * 60 * 60 * 1000L;
			if (System.currentTimeMillis() >= (date_firstLaunch + millisecondsToWait) || event_count >= eventsUntilPrompt) {
				if(date_reminder_pressed == 0){
                    if (prefs.getBoolean(PREF_APP_LOVE_CLICKED, true)){
                        showRateDialog(mContext, editor);
                    } else {
                        showLoveDialog(mContext, editor);
                    }
				}else{
					long remindMillisecondsToWait = daysBeforeReminding * 24 * 60 * 60 * 1000L;
					if(System.currentTimeMillis() >= (remindMillisecondsToWait + date_reminder_pressed)){
                        if (prefs.getBoolean(PREF_APP_LOVE_CLICKED, true)) {
                            showRateDialog(mContext, editor);
                        } else {
                            showLoveDialog(mContext, editor);
                        }
					}
				}
			}
        }
        
        editor.commit();
    }   

    public static void rateApp(Context mContext)
    {
        SharedPreferences prefs = mContext.getSharedPreferences(mContext.getPackageName()+".appirater", 0);
        SharedPreferences.Editor editor = prefs.edit();
        rateApp(mContext, editor);
    }

	@TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public static void significantEvent(Context mContext) {
        SharedPreferences prefs = mContext.getSharedPreferences(mContext.getPackageName()+".appirater", 0);
        if(!testMode && (prefs.getBoolean(PREF_DONT_SHOW, false) || prefs.getBoolean(PREF_RATE_CLICKED, false))) {return;}

        long event_count = prefs.getLong(PREF_EVENT_COUNT, 0);
        event_count++;
        prefs.edit().putLong(PREF_EVENT_COUNT, event_count).apply();
    }

    private static void rateApp(Context mContext, final SharedPreferences.Editor editor) {
        mContext.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(String.format(marketUrl, mContext.getPackageName()))));
        if (editor != null) {
            editor.putBoolean(PREF_RATE_CLICKED, true);
            editor.commit();
        }
    }

	@SuppressLint("NewApi")
	private static void showRateDialog(final Context mContext, final SharedPreferences.Editor editor) {
		Resources resources = mContext.getResources();
    	String appName = appTitle;

		int rateTitle = resources.getIdentifier("rate_title", "string", mContext.getPackageName());
		int rateMessage = resources.getIdentifier("rate_message", "string", mContext.getPackageName());
		int rateLater = resources.getIdentifier("rate_later", "string", mContext.getPackageName());
		int rate = resources.getIdentifier("rate", "string", mContext.getPackageName());
		int rateCancel = resources.getIdentifier("rate_cancel", "string", mContext.getPackageName());

		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setTitle(String.format(mContext.getString(rateTitle), appName));
		builder.setMessage(String.format(mContext.getString(rateMessage), appName));

		builder.setPositiveButton(String.format(mContext.getString(rate), appName), new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which) {
				rateApp(mContext, editor);
			}
		});

		builder.setNeutralButton(rateLater, new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				if (editor != null) {
					editor.putLong(PREF_DATE_REMINDER_PRESSED, System.currentTimeMillis());
					editor.commit();
				}
			}
		});

		builder.setNegativeButton(rateCancel, new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				if (editor != null) {
					editor.putBoolean(PREF_DONT_SHOW, true);
					editor.commit();
				}
			}
		});

		builder.setCancelable(false);

		final AlertDialog alertDialog = builder.create();
		alertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
			@Override
			public void onShow(DialogInterface dialog) {
				applyWorkaroundForButtonWidthsTooWide(mContext, alertDialog);
			}
		});
		alertDialog.show();
    }

	private static void resizeButton(Button button) {
		LinearLayout.LayoutParams params = (LinearLayout.LayoutParams)button.getLayoutParams();
		params.width = LinearLayout.LayoutParams.MATCH_PARENT;
		button.setLayoutParams(params);
	}

	private static void applyWorkaroundForButtonWidthsTooWide(Context mContext, AlertDialog alertDialog) {
		Button buttonOk = alertDialog.getButton(DialogInterface.BUTTON_POSITIVE);
		Button buttonLater = alertDialog.getButton(DialogInterface.BUTTON_NEUTRAL);
		Button buttonNo = alertDialog.getButton(DialogInterface.BUTTON_NEGATIVE);

		if (!(buttonOk.getParent() instanceof LinearLayout))
			return;

		LinearLayout linearLayout = (LinearLayout)buttonOk.getParent();
		linearLayout.setOrientation(LinearLayout.VERTICAL);
		linearLayout.setGravity(Gravity.RIGHT);

		linearLayout.removeAllViews();
		linearLayout.addView(buttonOk);
		linearLayout.addView(buttonLater);
		linearLayout.addView(buttonNo);

		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP)
		{
			resizeButton(buttonOk);
			resizeButton(buttonLater);
			resizeButton(buttonNo);
		}
	}

    private static void showLoveDialog(final Context mContext, final SharedPreferences.Editor editor) {
		Resources resources = mContext.getResources();

		int loveDialogContent = resources.getIdentifier("love_dialog_content", "string", mContext.getPackageName());
		int loveDialogYes = resources.getIdentifier("love_dialog_yes", "string", mContext.getPackageName());
		int loveDialogNo = resources.getIdentifier("love_dialog_no", "string", mContext.getPackageName());

		int loveDialogMessageId = resources.getIdentifier("love_dialog_message", "id", mContext.getPackageName());
		int loveDialogYesId = resources.getIdentifier("love_dialog_yes", "id", mContext.getPackageName());
		int loveDialogNoId = resources.getIdentifier("love_dialog_no", "id", mContext.getPackageName());

		int loveAppLayout = resources.getIdentifier("loveapp", "layout", mContext.getPackageName());

        final Dialog dialog = new Dialog(mContext);
        LinearLayout layout = (LinearLayout) LayoutInflater.from(mContext).inflate(loveAppLayout, null);

        TextView textView = (TextView)layout.findViewById(loveDialogMessageId);
        Button yesButton = (Button)layout.findViewById(loveDialogYesId);
        Button noButton = (Button)layout.findViewById(loveDialogNoId);

        textView.setText(String.format(mContext.getString(loveDialogContent)));
        yesButton.setText(String.format(mContext.getString(loveDialogYes)));
        noButton.setText(String.format(mContext.getString(loveDialogNo)));

        yesButton.setOnClickListener(new OnClickListener(){

            @Override
            public void onClick(View v) {
                if (editor != null) {
                    editor.putBoolean(PREF_APP_LOVE_CLICKED, true);
                    editor.commit();
                }
                dialog.dismiss();
                showRateDialog(mContext, editor);
            }
        });

        noButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (editor != null) {
                    editor.putBoolean(PREF_DONT_SHOW, true);
                    editor.commit();
                }
                dialog.dismiss();

                Intent intent = new Intent();
                intent.setAction("com.sbstrm.appirater.Appirater");
                intent.putExtra("HATE_APP", true);
                v.getContext().sendBroadcast(intent);
            }
        });


        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setContentView(layout, dialog.getWindow().getAttributes());
        dialog.show();
    }
}