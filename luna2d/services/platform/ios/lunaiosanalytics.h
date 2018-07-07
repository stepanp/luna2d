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

#include "lunaanalytics.h"

namespace luna2d{
	
class LUNAIosAnalyticsService : public LUNAAnalyticsService
{
public:
	LUNAIosAnalyticsService(id service);
	
private:
	id service;
	
public:
	// Send data to analytics
	virtual void Send(const std::string& event, const std::unordered_map<std::string,std::string>& data);
	
	virtual void StartEvent(const std::string& event);
	
	virtual void EndEvent(const std::string& event, const std::unordered_map<std::string,std::string>& data);
};

	
class LUNAIosAnalytics : public LUNAAnalytics
{
public:
	// Load service instance by name
	virtual std::shared_ptr<LUNAAnalyticsService> LoadService(const std::string& name);
};

}
