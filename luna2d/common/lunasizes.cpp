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

#include "lunasizes.h"
#include "lunaconfig.h"
#include "lunacamera.h"
#include "lunamath.h"
#include "lunaresolutions.h"
#include "lunarect.h"
#include "lunaplatformutils.h"

using namespace luna2d;

LUNASizes::LUNASizes(int physicalScreenWidth, int physicalScreenHeight, const std::shared_ptr<const LUNAConfig>& config)
{
	this->physicalScreenWidth = physicalScreenWidth;
	this->physicalScreenHeight = physicalScreenHeight;
	this->contentWidth = config->contentWidth;
	this->contentHeight = config->contentHeight;
	this->scaleMode = config->scaleMode;
	this->orientation = config->orientation;
	this->aspectRatio = physicalScreenWidth / (float)physicalScreenHeight;

	ApplyScaleMode(scaleMode);
	SelectResolution(config);

	textureScale = BASE_SIZE / (float)LUNASizes::GetHeightForResolution(resolutionSuffix);
}

void LUNASizes::ApplyScaleMode(LUNAScaleMode scaleMode)
{
	auto platformUtils = LUNAEngine::SharedPlatformUtils();
	int verticalMargins = platformUtils->GetTopScreenMargin() + platformUtils->GetBottomScreenMargin();
	float contentAspectRatio = orientation == LUNAOrientation::PORTRAIT ?
		physicalScreenWidth / (float)(physicalScreenHeight - verticalMargins) :
		(physicalScreenWidth - verticalMargins) / (float)physicalScreenHeight;

	if(scaleMode == LUNAScaleMode::STRETCH)
	{
		if(orientation == LUNAOrientation::PORTRAIT)
		{
			screenWidth = contentWidth;
			screenHeight = (int)(contentWidth / aspectRatio);
			contentHeight = (int)(contentWidth / contentAspectRatio);
		}
		else
		{
			screenHeight = contentHeight;
			screenWidth = (int)(contentHeight * aspectRatio);
			contentWidth = (int)(contentHeight * contentAspectRatio);
		}
	}

	else if(scaleMode == LUNAScaleMode::FIT)
	{
		if(orientation == LUNAOrientation::PORTRAIT)
		{
			screenHeight = contentHeight * (contentAspectRatio / aspectRatio);
			screenWidth = (int)(contentHeight * contentAspectRatio);
		}
		else
		{
			screenWidth = contentWidth / (contentAspectRatio / aspectRatio);
			screenHeight = (int)(contentWidth / contentAspectRatio);
		}
	}

	else if(scaleMode == LUNAScaleMode::ADAPTIVE)
	{
		if(orientation == LUNAOrientation::PORTRAIT)
		{
			if(contentWidth / contentAspectRatio < contentHeight) ApplyScaleMode(LUNAScaleMode::FIT);
			else ApplyScaleMode(LUNAScaleMode::STRETCH);
		}
		else
		{
			if(contentHeight * contentAspectRatio < contentWidth) ApplyScaleMode(LUNAScaleMode::FIT);
			else ApplyScaleMode(LUNAScaleMode::STRETCH);
		}
	}
}

// Select nearest texture resolution to screen resolution
void LUNASizes::SelectResolution(const std::shared_ptr<const LUNAConfig>& config)
{
	int screenSize = config->orientation == LUNAOrientation::PORTRAIT ? physicalScreenWidth : physicalScreenHeight;
	int count = config->resolutions.size();
	int minDiff = INT_MAX;
	int index = -1;

	for(int i = 0; i < count; i++)
	{
		int height = RESOLUTIONS_TABLE.at(config->resolutions[i]);
		int diff = std::fabs(screenSize - height);

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
	camera->SetPos(contentWidth / 2.0f, contentHeight / 2.0f);

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

// Get content width (in points)
int LUNASizes::GetContentWidth()
{
	return contentWidth;
}

// Get content height (in points)
int LUNASizes::GetContentHeight()
{
	return contentHeight;
}

// Get content area rect (in points)
LUNARect LUNASizes::GetContentRect()
{
	return LUNARect(0, 0, contentWidth, contentHeight);
}

// Get screen rect (in points)
LUNARect LUNASizes::GetScreenRect()
{
	float x = -(screenWidth - contentWidth) / 2.0f;
	float y = -(screenHeight - contentHeight) / 2.0f;

	return LUNARect(x, y, screenWidth, screenHeight);
}

// Get UI rect (in points)
LUNARect LUNASizes::GetUiRect()
{
	if(orientation == LUNAOrientation::PORTRAIT)
	{
		float x = -(screenWidth - contentWidth) / 2.0f;
		return LUNARect(x, 0, screenWidth, contentHeight);
	}
	else
	{
		float y = -(screenHeight - contentHeight) / 2.0f;
		return LUNARect(0, y, contentWidth, screenHeight);
	}
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
