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

#import "lunaiosservicesapi.h"
#include "lunaconfig.h"
#include "lunansstring.h"
#include "lunaplatformutils.h"
#import "lunaappdelegate.h"

using namespace luna2d;

@implementation LUNAIosServicesApi

// Check for config has value with given name
+(BOOL) hasConfigValue: (NSString*) name
{
	auto config = LUNAEngine::Shared()->GetConfig();
	return !config->GetCustomValues()[FromNsString(name)].is_null();
}

// Get string value from config
+(NSString*) getConfigString: (NSString*) name
{
	auto config = LUNAEngine::Shared()->GetConfig();
	return ToNsString(config->GetCustomValues()[FromNsString(name)].string_value());
}

// Get int value from config
+(int) getConfigInt: (NSString*) name
{
	auto config = LUNAEngine::Shared()->GetConfig();
	return config->GetCustomValues()[FromNsString(name)].number_value();
}

// Get float value from config
+(float) getConfigFloat: (NSString*) name
{
	auto config = LUNAEngine::Shared()->GetConfig();
	return config->GetCustomValues()[FromNsString(name)].number_value();
}

// Get bool value from config
+(BOOL) getConfigBool: (NSString*) name
{
	auto config = LUNAEngine::Shared()->GetConfig();
	return config->GetCustomValues()[FromNsString(name)].bool_value();
}

// Get array value from config
+(NSArray*) getConfigArray: (NSString*) name;
{
	auto config = LUNAEngine::Shared()->GetConfig();
	auto jsonArray = config->GetCustomValues()[FromNsString(name)].array_items();
	NSMutableArray* mutableArray = [NSMutableArray arrayWithCapacity: jsonArray.size()];
	
	for(const auto& jsonItem : jsonArray)
	{
		if(jsonItem.is_bool()) [mutableArray addObject: jsonItem.bool_value() ? @YES : @NO];
		else if(jsonItem.is_number()) [mutableArray addObject: [NSNumber numberWithDouble: jsonItem.number_value()]];
		else if(jsonItem.is_string()) [mutableArray addObject: ToNsString(jsonItem.string_value())];
	}
	
	return [mutableArray copy];
}

// Add delegate to handle events from UIApplication
+(void) addCustomAppDelegate: (id<LUNAIosCustomAppDelegate>) delegate
{
	LUNAAppDelegate* appDelegate = (LUNAAppDelegate*)[[UIApplication sharedApplication] delegate];
	[appDelegate.customDelegates addObject:delegate];
}

// Pause/resume engine with showing loading indicator during pause
+(void) setPauseWithLoadingIndicator: (BOOL) pause
{
	if(pause)
	{
		LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(true);
		LUNAEngine::Shared()->OnPause();
		
		// Protect game from resuming by operating system
		LUNAEngine::Shared()->EnablePauseHandling(false);
	}
	else
	{
		LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(false);
		LUNAEngine::Shared()->EnablePauseHandling(true);
		LUNAEngine::Shared()->OnResume();
	}
}

// Get launch options
+(NSDictionary*) getLaunchOptions
{
	LUNAAppDelegate* appDelegate = (LUNAAppDelegate*)[[UIApplication sharedApplication] delegate];
	return appDelegate.launchOptions;	
}

@end
