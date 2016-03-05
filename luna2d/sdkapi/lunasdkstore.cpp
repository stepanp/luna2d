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

#include "lunasdkstore.h"
#include "lunaconfig.h"
#include "lunaprefs.h"
#include "lunalog.h"

using namespace luna2d;

LUNASdkStore::LUNASdkStore()
{
	auto config = LUNAEngine::Shared()->GetConfig();
	auto prefs = LUNAEngine::SharedPrefs();

	if(config->HasCustomValue("store", "launchCount")) launchCount = config->GetCustomInt("store", "launchCount");

	// Calculate current launch count here
	// because module creates at launch
	curLaunchCount = prefs->GetInt("storeLaunchCount") + 1;
	if(curLaunchCount <= launchCount) prefs->SetInt("storeLaunchCount", curLaunchCount);
}

// Show rate app dialog immediately
void LUNASdkStore::ShowRateApp()
{
	DoRateApp();
}

// Request showing rate app dialog.
// Dialog will be showed at launch time specifed in "launchTimes" variable
void LUNASdkStore::RequestRateApp()
{
	if(curLaunchCount == launchCount && !dialogShowed)
	{
		dialogShowed = true;
		DoRateApp();
	}
}
