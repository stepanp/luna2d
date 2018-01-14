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

#include "lunaqtanalytics.h"

using namespace luna2d;

// Send data to analytics
void LUNAQtAnalyticsService::Send(const std::string& event, const std::unordered_map<std::string,std::string>& data)
{
	QHash<QString,QString> map;

	for(const auto& entry : data)
	{
		map.insert(QString::fromStdString(entry.first), QString::fromStdString(entry.second));
	}

	emit dataSent(QString::fromStdString(event), map);
}

LUNAQtAnalytics::LUNAQtAnalytics()
{
	service = std::make_shared<LUNAQtAnalyticsService>();
}

// Load service instance by name
std::shared_ptr<LUNAAnalyticsService> LUNAQtAnalytics::LoadService(const std::string& name)
{
	return nullptr;
}

std::shared_ptr<LUNAQtAnalyticsService> LUNAQtAnalytics::GetService()
{
	return std::static_pointer_cast<LUNAQtAnalyticsService>(service);
}
