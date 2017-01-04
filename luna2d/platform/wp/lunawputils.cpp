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

#include "lunawputils.h"
#include "lunawstring.h"
#include "lunalog.h"
#include <algorithm>
#include <windows.h>
#include <ppltasks.h>

using namespace Windows::System::UserProfile;
using namespace luna2d;

LUNAWpUtils::LUNAWpUtils(Windows::UI::Core::CoreDispatcher^ dispatcher) :
	dispatcher(dispatcher)
{
}

// Get system locale in "xx_XX" format
// Where "xx" is ISO-639 language code, and "XX" is ISO-3166 country code
std::string LUNAWpUtils::GetSystemLocale()
{
	std::string ret = FromPlatfromString(GlobalizationPreferences::Languages->GetAt(0));
	std::replace(ret.begin(), ret.end(), '-', '_');
	return std::move(ret);
}

// Open given url in system browser
void LUNAWpUtils::OpenUrl(const std::string& url)
{
	Windows::Foundation::Uri^ uri = nullptr;

	try
	{
		uri = ref new Windows::Foundation::Uri(ToPlatformString(url));
	}
	catch(Platform::InvalidArgumentException^ e)
	{
		LUNA_LOGE("Cannot open url: %s", url.c_str());
		return;
	}

	RunInUiThread([uri]() { concurrency::task<bool> startTask(Windows::System::Launcher::LaunchUriAsync(uri)); });
}

// Run given handler in UI thread
void LUNAWpUtils::RunInUiThread(std::function<void()> handler)
{
	dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler(handler));	
}