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

#include "lunaanalytics.h"
#include "lunaconfig.h"

using namespace luna2d;

// Load services from config
void LUNAAnalytics::LoadServices()
{
	auto config = LUNAEngine::Shared()->GetConfig();
	std::string serviceName = config->GetCustomValues()["analyticsService"].string_value();

	if(serviceName.empty()) return;

	service = LoadService(serviceName);
}

// Send data to analytics
void LUNAAnalytics::Send(const std::string& event, const std::unordered_map<std::string,std::string>& data)
{
	if(!service) return;

	service->Send(event, data);
}

void LUNAAnalytics::StartEvent(const std::string& event)
{
	if(!service) return;

	service->StartEvent(event);
}

void LUNAAnalytics::EndEvent(const std::string& event, const std::unordered_map<std::string,std::string>& data)
{
	if(!service) return;

	service->EndEvent(event, data);
}
