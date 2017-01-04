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

#pragma once

#include "lunaplatformutils.h"
#include "lunaandroidjni.h"

namespace luna2d{

//-----------------------------------------
// Platform utils implementaton for Android
//-----------------------------------------
class LUNAAndroidUtils : public LUNAPlatformUtils
{
public:
	LUNAAndroidUtils();

private:
	jclass javaUtils;
	jmethodID javaGetSystemLocale;
	jmethodID javaOpenUrl;
	jmethodID javaMessageDialog;
	jmethodID javaConfirmDialog;

	std::stack<std::function<void()>> messageCallbacks;
	std::stack<std::function<void(bool)>> confirmCallbacks;

public:
	std::function<void()> PopMessageCallback();
	std::function<void(bool)> PopConfirmCallback();

	// Get system locale in "xx_XX" format
	// Where "xx" is ISO-639 language code, and "XX" is ISO-3166 country code
	virtual std::string GetSystemLocale();

	// Open given url in system browser
	virtual void OpenUrl(const std::string& url);

	// Show native dialog with "Ok" button
	// "onClose" calls when dialog closed
	virtual void MessageDialog(const std::string& title, const std::string& message,
		const std::function<void()>& onClose);

	// Show native dialog with "Yes" and "No" buttons
	// "onClose" calls with "true" when "Yes" button pressed, and with "false" otherwise
	virtual void ConfirmDialog(const std::string& title, const std::string& message,
		const std::function<void(bool)>& onClose);

	// Show/hide loading indicator over game view
	virtual void ShowLoadingIndicator(bool show);
};

}
