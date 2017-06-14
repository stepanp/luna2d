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

#include "lunaengine.h"

namespace luna2d{

const std::string NOTIFICATIONS_DISABLED_ERR = "Notifications disabled. To enable notifications set \"enableNotifications\" in config to \"true\"";
const std::string NOTIFICATIONS_SECONDS_ERR = "Cannot schedule notification. Count of seconds must be positive";


class LUNANotifications
{
public:
    LUNANotifications();
	virtual ~LUNANotifications() {}

private:
    bool enabled = false;

public:
    // Check for notifications is enabled in config
    bool IsEnabled();

	// Schedule local notification
	virtual void Schedule(const std::string& message, int secondsFromNow, int id) = 0;

	// Cancel scheduled notification with specified id
	virtual void Cancel(int id) = 0;
};

}
