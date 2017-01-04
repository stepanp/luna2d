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

#include "lunaconfig.h"
#include "lunaglm.h"

namespace luna2d{

class LUNACamera;

class LUNASizes
{
public:
	LUNASizes(int physicalScreenWidth, int physicalScreenHeight, const std::shared_ptr<const LUNAConfig>& config);

private:
	int physicalScreenWidth, physicalScreenHeight; // Physical screen resolution (in pixels)
	int screenWidth, screenHeight; // Screen resolution (in points)
	int contentWidth, contentHeight; // Content size (in points)
	float textureScale;
	float aspectRatio;
	std::string resolutionSuffix;
	LUNAScaleMode scaleMode;

private:
	void ApplyScaleMode(LUNAScaleMode scaleMode);

	// Select nearest texture resolution to screen resolution
	void SelectResolution(const std::shared_ptr<const LUNAConfig>& config);

public:
	// Make instance of camera for renderer
	std::shared_ptr<LUNACamera> MakeCamera();

	// Get suffix for current resoltion
	const std::string& GetResolutionSuffix();

	// Get physical screen width (in pixels)
	int GetPhysicalScreenWidth();

	// Get physical screen height (in pixels)
	int GetPhysicalScreenHeight();

	// Get screen width (in points)
	int GetScreenWidth();

	// Get screen height (in points)
	int GetScreenHeight();

	// Get content width (in points)
	int GetContentWidth();

	// Get content height (in points)
	int GetContentHeight();

	// Get aspect ratio
	float GetAspectRatio();

	// Get scale ratio between screen resolution (in points) and texture resolution (in pixels)
	float GetTextureScale();

	// Get scale mode
	LUNAScaleMode GetScaleMode();

	// Get height value for given resolution suffix
	// Return -1 if resolution not found
	static int GetHeightForResolution(const std::string& resolutionSuffix);
};

}
