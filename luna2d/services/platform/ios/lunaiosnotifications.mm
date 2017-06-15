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

//@property (nonatomic, strong) isAccessStateUnknown;

-(void) requestAccess;

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow id: (int)id;

-(void) cancel: (int)id;

@end

@implementation Notifications10

-(void) requestAccess
{
	UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
	[center requestAuthorizationWithOptions:(UNAuthorizationOptionBadge | UNAuthorizationOptionSound | UNAuthorizationOptionAlert)
						  completionHandler:^(BOOL, NSError* error) {}];
}

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow id: (int)id
{
	UNMutableNotificationContent* notification = [[UNMutableNotificationContent alloc] init];
    notification.body = message;
    notification.sound = [UNNotificationSound defaultSound];
    notification.badge = [NSNumber numberWithInt:1];

    UNTimeIntervalNotificationTrigger* trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:secondsFromNow repeats:NO];
    UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier:[@(id) stringValue] content:notification trigger:trigger];
    
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center addNotificationRequest:request withCompletionHandler:nil];
}

-(void) cancel: (int)id
{
	NSArray* ids = [NSArray arrayWithObject:[@(id) stringValue]];
	
	UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
	[center removePendingNotificationRequestsWithIdentifiers:ids];
}
	
@end


//-------------------------------------------------
// Notifications implementation for iOS version 8-9
//-------------------------------------------------
@interface Notifications89 : NSObject

-(void) requestAccess;

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow id: (int)id;

-(void) cancel: (int)id;
	
@end

@implementation Notifications89

-(void) requestAccess
{
	UIUserNotificationSettings* settings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge | UIUserNotificationTypeAlert | UIUserNotificationTypeSound categories:nil];
	[[UIApplication sharedApplication] registerUserNotificationSettings:settings];
}

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow id: (int)id
{
	// Cancel previous notification with same id
	[self cancel:id];
	
	UILocalNotification* notification = [[UILocalNotification alloc] init];

	notification.fireDate = [NSDate dateWithTimeIntervalSinceNow:secondsFromNow];
	notification.timeZone = [NSTimeZone defaultTimeZone];
	notification.alertBody = message;
	notification.alertAction = nil;
	notification.applicationIconBadgeNumber = 1;
	notification.soundName = UILocalNotificationDefaultSoundName;
	notification.userInfo = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:id] forKey:@"id"];
	
	[[UIApplication sharedApplication] scheduleLocalNotification:notification];
}

-(void) cancel: (int)id
{
	for(UILocalNotification* notification : [[UIApplication sharedApplication] scheduledLocalNotifications])
	{
		if([[notification.userInfo objectForKey:@"id"] isEqualToNumber:[NSNumber numberWithInt:id]])
		{
			[[UIApplication sharedApplication] cancelLocalNotification:notification];
		}
	}
}
	
@end


LUNAIosNotifications::LUNAIosNotifications() : LUNANotifications()
{
	if(!IsEnabled()) return;

	if([[[UIDevice currentDevice] systemVersion] floatValue] < 10.0) impl = [[Notifications89 alloc] init];
	else impl = [[Notifications10 alloc] init];

	[impl requestAccess];
}

// Schedule local notification
void LUNAIosNotifications::Schedule(const std::string& message, int secondsFromNow, int id)
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());
	if(secondsFromNow <= 0) LUNA_RETURN_ERR(NOTIFICATIONS_SECONDS_ERR.c_str());
	
	[impl schedule:ToNsString(message) secondsFromNow:secondsFromNow id:id];
}

// Cancel scheduled notification with specified id
void LUNAIosNotifications::Cancel(int id)
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());

	[impl cancel:id];
}

// Suppress notification if it caused while application in foreground
void LUNAIosNotifications::SuppressWhileForeground(UILocalNotification* notification)
{
	[UIApplication sharedApplication].applicationIconBadgeNumber = 1; // It's not misprint, didn't work without it
	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
	[[UIApplication sharedApplication] cancelLocalNotification:notification];
}
