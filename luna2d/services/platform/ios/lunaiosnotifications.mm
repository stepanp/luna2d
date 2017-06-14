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

#include "lunaiosnotifications.h"
#include "lunansstring.h"
#include "lunalog.h"
#include "lunamacro.h"
#import <UserNotifications/UserNotifications.h>

using namespace luna2d;

//---------------------------------------------------
// Notifications implementation for iOS version >= 10
//---------------------------------------------------
@interface Notifications10 : NSObject

-(void) requestAccess;

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow;

-(void) cancel;

@end

@implementation Notifications10

-(void) requestAccess
{
	UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
	[center requestAuthorizationWithOptions:(UNAuthorizationOptionBadge | UNAuthorizationOptionSound | UNAuthorizationOptionAlert) completionHandler:nil];
}

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow
{
	UNMutableNotificationContent* notification = [[UNMutableNotificationContent alloc] init];
    notification.body = message;
    notification.sound = [UNNotificationSound defaultSound];
    notification.badge = [NSNumber numberWithInt:1];

    UNTimeIntervalNotificationTrigger* trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:secondsFromNow repeats:NO];
    UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier:@"0" content:notification trigger:trigger];
    
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center addNotificationRequest:request withCompletionHandler:nil];
}

-(void) cancel
{
	UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
	[center removeAllPendingNotificationRequests];
}
	
@end


//-------------------------------------------------
// Notifications implementation for iOS version 8-9
//-------------------------------------------------
@interface Notifications89 : NSObject

-(void) requestAccess;

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow;

-(void) cancel;
	
@end

@implementation Notifications89

-(void) requestAccess
{
	UIUserNotificationSettings* settings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge | UIUserNotificationTypeAlert | UIUserNotificationTypeSound categories:nil];
	[[UIApplication sharedApplication] registerUserNotificationSettings:settings];
}

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow
{
	UILocalNotification* notification = [[UILocalNotification alloc] init];

	notification.fireDate = [NSDate dateWithTimeIntervalSinceNow:secondsFromNow];
	notification.timeZone = [NSTimeZone defaultTimeZone];
	notification.alertBody = message;
	notification.alertAction = @"";
	notification.applicationIconBadgeNumber = 1;
	notification.soundName = UILocalNotificationDefaultSoundName;
	
	[[UIApplication sharedApplication] scheduleLocalNotification:notification];
}

-(void) cancel
{
	for(UILocalNotification* notification : [[UIApplication sharedApplication] scheduledLocalNotifications])
	{
		 [[UIApplication sharedApplication] cancelLocalNotification:notification];
	}
}
	
@end


LUNAIosNotifications::LUNAIosNotifications() : LUNANotifications()
{
	if(!IsEnabled()) return;

	if([[[UIDevice currentDevice] systemVersion] floatValue] < 10.0) impl = [[Notifications89 alloc] init];
	else impl = [[Notifications10 alloc] init];

	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
	[impl requestAccess];
}

// Schedule local push notification
void LUNAIosNotifications::Schedule(const std::string& message, int secondsFromNow)
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());
	if(secondsFromNow <= 0) LUNA_RETURN_ERR(NOTIFICATIONS_SECONDS_ERR.c_str());

	[impl schedule:ToNsString(message) secondsFromNow:secondsFromNow];
}

// Cancel scheduled notifications
void LUNAIosNotifications::Cancel()
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());

	[impl cancel];
}

// Suppress notification if it caused while application in foreground
void LUNAIosNotifications::SuppressWhileForeground()
{
	Cancel();
	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
}
