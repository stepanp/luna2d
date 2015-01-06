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
#include "lunasizes.h"
#include <functional>
#include <algorithm>

using namespace luna2d;

// Init with default values
LUNAConfig::LUNAConfig() :
	orientation(LUNAOrientation::LANDSCAPE),
	scaleMode(LUNAScaleMode::FIT_TO_HEIGHT_LEFT),
	baseWidth(480), baseHeight(BASE_SIZE)
{
}

// Init with config from "config.lua" file
LUNAConfig::LUNAConfig(LuaTable tblConfig) : LUNAConfig()
{
	Read(tblConfig);
}

void LUNAConfig::Read(LuaTable tblConfig)
{
	if(tblConfig.HasField("orientation"))
	{
		std::string str = tblConfig.GetString("orientation");
		if(str == "portrait") orientation = LUNAOrientation::PORTRAIT;
		else if(str == "landscape") orientation = LUNAOrientation::LANDSCAPE;
		else LUNA_LOGE("Incorrect orientation \"%s\"", str.c_str());
	}

	if(tblConfig.HasField("resolutions"))
	{
		resolutions = tblConfig.GetField<std::vector<std::string>>("resolutions");

		// Filter unsupported resolutions
		auto predicate = [](const std::string& res)
		{
			if(RESOLUTIONS_TABLE.count(res) == 0)
			{
				LUNA_LOGE("Unsupported resolution \"%s\"", res.c_str());
				return true;
			}

			return false;
		};
		resolutions.erase(std::remove_if(resolutions.begin(), resolutions.end(), predicate), resolutions.end());
	}

	if(tblConfig.HasField("scaleMode"))
	{
		std::string scaleModeStr = tblConfig.GetString("scaleMode");
		if(scaleModeStr == "fitToHeightLeft") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_LEFT;
		else if(scaleModeStr == "fitToHeightRight") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_RIGHT;
		else if(scaleModeStr == "fitToHeightCenter") scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_CENTER;
		else if(scaleModeStr == "fitToWidthTop") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_TOP;
		else if(scaleModeStr == "fitToWidthBottom") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_BOTTOM;
		else if(scaleModeStr == "fitToWidthCenter") scaleMode = LUNAScaleMode::FIT_TO_WIDTH_CENTER;
		else LUNA_LOGE("Unsupported scale mode \"%s\"", scaleModeStr.c_str());
	}

	if(tblConfig.HasField("baseHeight")) baseHeight = tblConfig.GetFloat("baseHeight");
	if(tblConfig.HasField("baseWidth")) baseWidth = tblConfig.GetFloat("baseWidth");
}
