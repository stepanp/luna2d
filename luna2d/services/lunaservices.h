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

#include "lunaengine.h"

namespace luna2d{

class LUNAAds;
class LUNAPurchases;
class LUNASharing;
class LUNAStore;
class LUNALeaderboards;
class LUNANotifications;

class LUNAServices
{
public:
	virtual ~LUNAServices() {}

protected:
	std::shared_ptr<LUNAAds> ads;
	std::shared_ptr<LUNAPurchases> purchases;
	std::shared_ptr<LUNASharing> sharing;
	std::shared_ptr<LUNAStore> store;
	std::shared_ptr<LUNALeaderboards> leaderboards;
	std::shared_ptr<LUNANotifications> notifications;

public:
	std::shared_ptr<LUNAAds> GetAds();
	std::shared_ptr<LUNAPurchases> GetPurchases();
	std::shared_ptr<LUNASharing> GetSharing();
	std::shared_ptr<LUNAStore> GetStore();
	std::shared_ptr<LUNALeaderboards> GetLeaderboards();
	std::shared_ptr<LUNANotifications> GetNotifications();

	virtual void LoadServices() = 0;
};

}
