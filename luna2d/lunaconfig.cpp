//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaconfig.h"
#include "lunaengine.h"
#include "lunalog.h"
#include "lunafiles.h"
#include "lunasizes.h"
#include <functional>
#include <algorithm>
#include <json11.hpp>

using namespace luna2d;
using namespace json11;

void LUNAConfig::Read()
{
	if(!LUNAEngine::SharedFiles()->IsExists(CONFIG_FILENAME))
	{
		LUNA_LOGW("\"%s\" not found. Use default config", CONFIG_FILENAME.c_str());
		return;
	}

	std::string configData = LUNAEngine::SharedFiles()->ReadFileToString(CONFIG_FILENAME, LUNAFileLocation::ASSETS);
	std::string err;
	Json jsonConfig = Json::parse(configData, err);
	if(jsonConfig == nullptr)
	{
		LUNA_LOGE("Error with parsing config: \"%s\"", err.c_str());
		return;
	}

	if(jsonConfig["orientation"] != nullptr)
	{
		std::string str = jsonConfig["orientation"].string_value();
		if(str == "portrait") orientation = LUNAOrientation::PORTRAIT;
		else if(str == "landscape") orientation = LUNAOrientation::LANDSCAPE;
		else LUNA_LOGE("Incorrect orientation \"%s\"", str.c_str());
	}

	if(jsonConfig["resolutions"] != nullptr)
	{
		auto jsonResolutions = jsonConfig["resolutions"].array_items();
		std::vector<std::string> parsedResolutions;

		// Parse resolutions from config
		for(auto item : jsonResolutions)
		{
			std::string res = item.string_value();

			if(RESOLUTIONS_TABLE.count(res) == 0)
			{
				LUNA_LOGE("Unsupported resolution \"%s\"", res.c_str());
				continue;
			}

			parsedResolutions.push_back(res);
		}

		// Replace default resolutions list with resolutions from config
		if(!parsedResolutions.empty()) resolutions.swap(parsedResolutions);
	}

	if(jsonConfig["scaleMode"] != nullptr)
	{
		std::string scaleModeStr = jsonConfig["scaleMode"].string_value();
		if(scaleModeStr == "fitToHeightLeft") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_LEFT;
		else if(scaleModeStr == "fitToHeightRight") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_RIGHT;
		else if(scaleModeStr == "fitToHeightCenter") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_CENTER;
		else if(scaleModeStr == "fitToWidthTop") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_TOP;
		else if(scaleModeStr == "fitToWidthBottom") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_BOTTOM;
		else if(scaleModeStr == "fitToWidthCenter") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_CENTER;
		else LUNA_LOGE("Unsupported scale mode \"%s\"", scaleModeStr.c_str());
	}

	if(jsonConfig["baseWidth"] != nullptr)
	{
		if(jsonConfig["baseWidth"].is_number()) baseWidth = jsonConfig["baseWidth"].int_value();
		else LUNA_LOGE("Base width must be number");
	}

	if(jsonConfig["baseHeight"] != nullptr)
	{
		if(jsonConfig["baseHeight"].is_number()) baseHeight = jsonConfig["baseHeight"].int_value();
		else LUNA_LOGE("Base height must be number");
	}
}
