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

#include "lunasharing.h"
#include "lunalog.h"
#include "lunaconfig.h"

using namespace luna2d;

std::shared_ptr<LUNASharingService> LUNASharing::GetService(const std::string& serviceName)
{
	auto it = services.find(serviceName);
	if(it == services.end())
	{
		LUNA_LOGE("Sharing service with name \"%s\" not found", serviceName.c_str());
		return nullptr;
	}
	return it->second;
}

// Load services from config
void LUNASharing::LoadServices()
{
	services[""] = LoadService(""); // Load default service

	auto config = LUNAEngine::Shared()->GetConfig();
	auto jsonServices = config->GetCustomValues()["sharingServices"].array_items();

	for(const auto& jsonName : jsonServices)
	{
		auto service = LoadService(jsonName.string_value());
		if(service) services[service->GetName()] = service;
	}
}

// Share given text using specified service. If service is not specifed system sharing dialog will be used
void LUNASharing::Text(const std::string& text, const std::string& serviceName)
{
	auto service = GetService(serviceName);
	if(service) service->Text(text);
}

// Share given image witg given text using specified service. If service is not specifed system sharing dialog will be used
void LUNASharing::Image(const std::string& filename, const std::string& text, const std::string& serviceName)
{
	auto service = GetService(serviceName);
	if(service) service->Image(filename, text);
}
