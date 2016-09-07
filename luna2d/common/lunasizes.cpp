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

#include "lunasizes.h"
#include "lunaconfig.h"
#include "lunacamera.h"
#include "lunamath.h"
#include "lunaresolutions.h"

using namespace luna2d;

LUNASizes::LUNASizes(int physicalScreenWidth, int physicalScreenHeight, const std::shared_ptr<const LUNAConfig>& config)
{
	this->physicalScreenWidth = physicalScreenWidth;
	this->physicalScreenHeight = physicalScreenHeight;
	this->gameAreaWidth = config->gameAreaWidth;
	this->gameAreaHeight = config->gameAreaHeight;
	this->scaleMode = config->scaleMode;
	this->aspectRatio = physicalScreenWidth / (float)physicalScreenHeight;

	ApplyScaleMode(scaleMode);
	SelectResolution(config);

	textureScale = BASE_SIZE / (float)LUNASizes::GetHeightForResolution(resolutionSuffix);
}

void LUNASizes::ApplyScaleMode(LUNAScaleMode scaleMode)
{
	switch(scaleMode)
	{
	case LUNAScaleMode::STRETCH_BY_WIDTH:
		screenHeight = gameAreaHeight;
		screenWidth = (int)(gameAreaHeight * aspectRatio);
		gameAreaWidth = screenWidth;
		break;
	case LUNAScaleMode::STRETCH_BY_HEIGHT:
		screenWidth = gameAreaWidth;
		screenHeight = (int)(gameAreaWidth / aspectRatio);
		gameAreaHeight = screenHeight;
		break;
	case LUNAScaleMode::FIT_TO_WIDTH:
		screenWidth = gameAreaWidth;
		screenHeight = (int)(gameAreaWidth / aspectRatio);
		break;
	case LUNAScaleMode::FIT_TO_HEIGHT:
		screenHeight = gameAreaHeight;
		screenWidth = (int)(gameAreaHeight * aspectRatio);
		break;
	};
}

// Select nearest texture resolution to screen resolution
void LUNASizes::SelectResolution(const std::shared_ptr<const LUNAConfig>& config)
{
	int count = config->resolutions.size();
	int minDiff = INT_MAX;
	int index = -1;

	for(int i = 0; i < count; i++)
	{
		int height = RESOLUTIONS_TABLE.at(config->resolutions[i]);
		int diff = std::fabs(physicalScreenHeight - height);

		if(diff < minDiff)
		{
			minDiff = diff;
			index = i;
		}
	}

	resolutionSuffix = config->resolutions[index];
}

// Make instance of camera for renderer
std::shared_ptr<LUNACamera> LUNASizes::MakeCamera()
{
	auto camera = std::make_shared<LUNACamera>(screenWidth, screenHeight);
	camera->SetPos(gameAreaWidth / 2.0f, gameAreaHeight / 2.0f);

	return camera;
}

// Get suffix for current resoltion
const std::string& LUNASizes::GetResolutionSuffix()
{
	return resolutionSuffix;
}

// Get physical screen width (in pixels)
int LUNASizes::GetPhysicalScreenWidth()
{
	return physicalScreenWidth;
}

// Get physical screen height (in pixels)
int LUNASizes::GetPhysicalScreenHeight()
{
	return physicalScreenHeight;
}

// Get screen width (in points)
int LUNASizes::GetScreenWidth()
{
	return screenWidth;
}

// Get screen height (in points)
int LUNASizes::GetScreenHeight()
{
	return screenHeight;
}

// Get game area width (in points)
int LUNASizes::GetGameAreaWidth()
{
	return gameAreaWidth;
}

// Get game area height (in points)
int LUNASizes::GetGameAreaHeight()
{
	return gameAreaHeight;
}

// Get aspect ratio
float LUNASizes::GetAspectRatio()
{
	return aspectRatio;
}

// Get scale ratio between screen resolution (in points) and texture resolution (in pixels)
float LUNASizes::GetTextureScale()
{
	return textureScale;
}

// Get scale mode
LUNAScaleMode LUNASizes::GetScaleMode()
{
	return scaleMode;
}

// Get height value for given resolution suffix
// Return -1 if resolution not found
int LUNASizes::GetHeightForResolution(const std::string& resolutionSuffix)
{
	if(RESOLUTIONS_TABLE.count(resolutionSuffix) == 0) return -1;
	return RESOLUTIONS_TABLE.at(resolutionSuffix);
}
