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

#include "lunaconfig.h"
#include "lunaengine.h"
#include "lunalog.h"
#include "lunafiles.h"
#include "lunasizes.h"
#include "lunajsonutils.h"

using namespace luna2d;
using namespace json11;

void LUNAConfig::ReadScreenOrientation(const json11::Json& jsonConfig)
{
	auto jsonOrientation = jsonConfig["orientation"];
	if(jsonOrientation.is_null()) return;

	std::string orientationStr = jsonOrientation.string_value();
	if(ORIENTATION.HasKey(orientationStr)) orientation = ORIENTATION.FromString(orientationStr);
	else LUNA_LOGE("Incorrect orientation \"%s\"", orientationStr.c_str());
}

void LUNAConfig::ReadResolutions(const json11::Json& jsonConfig)
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

void LUNAConfig::ReadScaleMode(const json11::Json& jsonConfig)
{
	auto jsonScaleMode = jsonConfig["scaleMode"];
	if(jsonScaleMode.is_null()) return;

	std::string scaleModeStr = jsonScaleMode.string_value();
	if(!SCALE_MODE.HasKey(scaleModeStr)) LUNA_LOGE("Unsupported scale mode \"%s\"", scaleModeStr.c_str());
	else scaleMode = SCALE_MODE.FromString(scaleModeStr);
}

void LUNAConfig::ReadContentWidth(const json11::Json& jsonConfig)
{
	auto jsonContentWidth = jsonConfig["contentWidth"];
	if(jsonContentWidth.is_null()) return;

	if(jsonContentWidth.is_number()) contentWidth = jsonContentWidth.int_value();
	else LUNA_LOGE("Content width must be number");
}

void LUNAConfig::ReadContentHeight(const json11::Json& jsonConfig)
{
	auto jsonContentHeight = jsonConfig["contentHeight"];
	if(jsonContentHeight.is_null()) return;

	if(jsonContentHeight.is_number()) contentHeight = jsonContentHeight.int_value();
	else LUNA_LOGE("Content height must be number");
}

void LUNAConfig::ReadDebugValues(const json11::Json& jsonConfig)
{
	debug_missedStrings = jsonConfig["debug_missedStrings"].bool_value();
}

const Json& LUNAConfig::GetCustomValues() const
{
	return customValues;
}

bool LUNAConfig::Read()
{
	if(!LUNAEngine::SharedFiles()->IsExists(CONFIG_FILENAME))
	{
		LUNA_LOGW("Config file \"%s\" not found", CONFIG_FILENAME.c_str());
		return false;
	}

	std::string configData = LUNAEngine::SharedFiles()->ReadFileToString(CONFIG_FILENAME, LUNAFileLocation::ASSETS);
	std::string err;
	Json jsonConfig = Json::parse(configData, err, JsonParse::COMMENTS);

	if(jsonConfig == nullptr)
	{
		LUNA_LOGE("Error with parsing config: \"%s\"", err.c_str());
		return false;
	}

	if(jsonConfig["name"] == nullptr)
	{
		LUNA_LOGE("\"name\" field not found. It's required field");
		return false;
	}

	gameName = jsonConfig["name"].string_value();
	if(gameName.empty())
	{
		LUNA_LOGE("Invalid name");
		return false;
	}

	ReadScreenOrientation(jsonConfig);

	// Set default config values for portrait mode if it set
	if(orientation == LUNAOrientation::PORTRAIT)
	{
		std::swap(contentWidth, contentHeight);
		scaleMode = LUNAScaleMode::STRETCH_BY_HEIGHT;
	}

	ReadResolutions(jsonConfig);
	ReadScaleMode(jsonConfig);
	ReadContentWidth(jsonConfig);
	ReadContentHeight(jsonConfig);
	ReadDebugValues(jsonConfig);

	customValues = jsonConfig;

	return true;
}
