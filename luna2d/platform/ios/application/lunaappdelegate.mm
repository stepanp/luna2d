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

#import "lunaappdelegate.h"
#import "lunaviewcontroller.h"
#import "lunaservices.h"
#import "lunaiosnotifications.h"
#include "lunaengine.h"

using namespace luna2d;

@interface LUNAAppDelegate ()

@end

@implementation LUNAAppDelegate

-(BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
	[[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:nil];
	
	self.customDelegates = [[NSMutableArray alloc] init];
	self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	self.window.rootViewController = [[LUNAViewController alloc] initWithNibName:nil bundle:nil];
	[self.window makeKeyAndVisible];
	
	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
	self.launchOptions = launchOptions;
	
	return YES;
}

-(void) application:(UIApplication*)application didReceiveLocalNotification:(UILocalNotification*)notification
{
	auto notifications = std::static_pointer_cast<LUNAIosNotifications>(LUNAEngine::SharedServices()->GetNotifications());
	notifications->SuppressWhileForeground(notification);
}

-(void) applicationDidEnterBackground:(UIApplication*)application
{
	LUNAEngine::Shared()->OnPause();
	
	for(id delegate in self.customDelegates)
	{
		[delegate applicationDidEnterBackground:application];
	}
}

-(void) applicationWillEnterForeground:(UIApplication*)application
{
	LUNAEngine::Shared()->OnResume();
	
	[UIApplication sharedApplication].applicationIconBadgeNumber = 0;
	
	for(id delegate in self.customDelegates)
	{
		[delegate applicationWillEnterForeground:application];
	}
}

-(void) applicationDidBecomeActive:(UIApplication*)application
{
	for(id delegate in self.customDelegates)
	{
		[delegate applicationDidBecomeActive:application];
	}
}

-(BOOL) application:(UIApplication*)application continueUserActivity:(NSUserActivity*)userActivity restorationHandler:(void (^)(NSArray*))restorationHandler
{
	for(id delegate in self.customDelegates)
	{
		if(![delegate application:application continueUserActivity:userActivity restorationHandler:restorationHandler]) return NO;
	}
	
	return YES;
}

-(BOOL) application:(UIApplication*)application openURL:(NSURL*)url options:(NSDictionary<NSString*,id>*)options
{
	for(id delegate in self.customDelegates)
	{
		if(![delegate application:application openURL:url options:options]) return NO;
	}

	return YES;
}

-(BOOL) application:(UIApplication*)application openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:(id)annotation
{
	for(id delegate in self.customDelegates)
	{
		if(![delegate application:application openURL:url sourceApplication:sourceApplication annotation:annotation]) return NO;
	}
	
	return YES;
}

@end

int main(int argc, char* argv[])
{
	@autoreleasepool
	{
		return UIApplicationMain(argc, argv, nil, NSStringFromClass([LUNAAppDelegate class]));
	}
}
