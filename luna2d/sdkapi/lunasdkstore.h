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

#pragma once

#include "lunabasesdk.h"

namespace luna2d{

class LUNASdkStore : public LUNABaseSdk
{
public:
	LUNASdkStore();

protected:
	// Show rate app dialog
	// Should be implemented in platform-specific sdk module
	virtual void DoRateApp() = 0;

protected:
	int launchCount = 2; // Launch count when need show rate app dialog
	int curLaunchCount = 0; // Current launch count
	bool dialogShowed = false;

public:
	// Show rate app dialog immediately
	void ShowRateApp();

	// Request showing rate app dialog.
	// Dialog will be showed at launch count specifed in "launchTimes" variable
	void RequestRateApp();

	// This method calls when user clicks "Rate later" button in rate app dialog
	void OnUserPostpone();
};

}
