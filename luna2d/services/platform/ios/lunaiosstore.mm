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

#include "lunaiosstore.h"
#include "lunansstring.h"
#include "lunalog.h"
#import "lunaiosservicesapi.h"
#import <Appirater.h>

using namespace luna2d;

// Get url to page of game in store
std::string LUNAIosStore::GetUrl()
{
	NSString* appName = [NSString stringWithString:[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"]];
	NSString* url = [NSString stringWithFormat:@"https://appstore.com/%@",
		[[appName stringByReplacingOccurrencesOfString:@" " withString:@""] lowercaseString]];

	return FromNsString(url);
}

// Open page of game in store
void LUNAIosStore::OpenPage()
{
	NSString* appName = [NSString stringWithString:[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"]];
	NSURL* url = [NSURL URLWithString:[NSString stringWithFormat:@"itms-apps://itunes.com/app/%@",
		[appName stringByReplacingOccurrencesOfString:@" " withString:@""]]];
	[[UIApplication sharedApplication] openURL:url];
}

// Request rate app dialog
void LUNAIosStore::RequestRateApp()
{
	NSString* appId = @"";
	int days = 0;
	int launches = 2;
	int remindingTime = 2;
	
	if([LUNAIosServicesApi hasConfigValue:@"rateAppId"]) appId = [LUNAIosServicesApi getConfigString:@"rateAppId"];
	if([LUNAIosServicesApi hasConfigValue:@"rateAppDays"]) days = [LUNAIosServicesApi getConfigInt:@"rateAppDays"];
	if([LUNAIosServicesApi hasConfigValue:@"rateAppLaunches"]) launches = [LUNAIosServicesApi getConfigInt:@"rateAppLaunches"];
	if([LUNAIosServicesApi hasConfigValue:@"rateAppRemindingTime"]) remindingTime = [LUNAIosServicesApi getConfigInt:@"rateAppRemindingTime"];
	
	[Appirater setAppId:appId];
	[Appirater setDaysUntilPrompt:days];
	[Appirater setUsesUntilPrompt:launches];
	[Appirater setSignificantEventsUntilPrompt:-1];
	[Appirater setTimeBeforeReminding:remindingTime];
	[Appirater appLaunched:YES];
}


