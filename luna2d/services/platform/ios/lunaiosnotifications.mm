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

using namespace luna2d;

//---------------------------------------------------
// Notifications implementation for iOS version >= 10
//---------------------------------------------------
@interface Notifications10 : NSObject

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow;

-(void) cancel;

-(void) register;

@end

@implementation Notifications10

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow
{

}

-(void) cancel
{

}

-(void) register
{

}
	
@end


//-------------------------------------------------
// Notifications implementation for iOS version 8-9
//-------------------------------------------------
@interface Notifications89 : NSObject

-(void) schedule: (NSString*)message secondsFromNow: (int)secondsFromNow;

-(void) cancel;

-(void) register;
	
@end

@implementation Notifications89

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

-(void) register
{
	UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge | UIUserNotificationTypeAlert | UIUserNotificationTypeSound categories:nil];
	[[UIApplication sharedApplication] registerUserNotificationSettings:settings];
}
	
@end


LUNAIosNotifications::LUNAIosNotifications()
{
	if([[[UIDevice currentDevice] systemVersion] floatValue] < 10.0) impl = [[Notifications89 alloc] init];
	else impl = [[Notifications10 alloc] init];

	[impl register];
}

// Schedule local push notification
void LUNAIosNotifications::Schedule(const std::string& message, int secondsFromNow)
{
	if(secondsFromNow <= 0) return;

	[impl schedule:ToNsString(message) secondsFromNow:secondsFromNow];
}

// Cancel scheduled notifications
void LUNAIosNotifications::Cancel()
{
	[impl cancel];
}
