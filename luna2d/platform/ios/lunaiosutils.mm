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

#include "lunaiosutils.h"
#include "lunalog.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

using namespace luna2d;

// Get system locale in "xx_XX" format
// Where "xx" is ISO-639 language code, and "XX" is ISO-3166 country code
std::string LUNAIosUtils::GetSystemLocale()
{
	std::string ret = [[[NSLocale preferredLanguages] objectAtIndex:0] UTF8String];
	std::replace(ret.begin(), ret.end(), '-', '_');
	return ret;
}

// Open given url in system browser
void LUNAIosUtils::OpenUrl(const std::string& url)
{
	NSURL* nsUrl = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
	
	if(![[UIApplication sharedApplication] openURL:nsUrl]) LUNA_LOGE("Cannot open url: %s", url.c_str());
}

// Show native dialog with "Ok" button
// "onClose" calls when dialog closed
void LUNAIosUtils::MessageDialog(const std::string& title, const std::string& message,
	const std::function<void()>& onClose)
{
	LUNA_LOGE("Method LUNAIosUtils::MessageDialog is not implemented");
	
	if(onClose) onClose();
}

// Show native dialog with "Yes" and "No" buttons
// "onClose" calls with "true" when "Yes" button pressed, and with "false" otherwise
void LUNAIosUtils::ConfirmDialog(const std::string& title, const std::string& message,
	const std::function<void(bool)>& onClose)
{
	LUNA_LOGE("Method LUNAIosUtils::ConfirmDialog is not implemented");
	
	if(onClose) onClose(false);
}