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

#pragma once

#include "lunaconfig.h"
#include "lunaglm.h"
#include "lunaresolutions.h"

namespace luna2d{

class LUNASizes
{
public:
	LUNASizes(int screenWidth, int screenHeight, LUNAConfig* config);

private:
	int physicalWidth, physicalHeight; // Physical screen resolution
	int virtualWidth, virtualHeight; // Virtual screen resolution
	int baseWidth, baseHeight;
	float scaleFactor; // Scale ratio between virtual and physical screen resolutions
	float aspectRatio;
	float textureScale; // Scale ratio between virtual and texture resolutions
	std::string resolutionSuffix;
	LUNAScaleMode scaleMode;
	glm::mat4 transformMatrix;

private:
	void SelectResolution(LUNAConfig *config);
	void BuildTransformMatrix(); // Build transformation matrix by scale mode

public:
	const glm::mat4& GetTransformMatrix(); // Get transformation matrix for renderer
	std::string GetResolutionSuffix(); // Get suffix for current resoltion
	int GetPhysicalScreenWidth(); // Get physical screen width
	int GetPhysicalScreenHeight(); // Get physical screen height
	int GetVirtualScreenWidth(); // Get virtual screen width
	int GetVirtualScreenHeight(); // Get virtual screen height
	int GetBaseScreenWidth(); // Get base screen width
	int GetBaseScreenHeight(); // Get base screen height
	float GetAspectRatio(); // Get aspect ratio
	float GetScaleFactor(); // Get scale ratio between virtual and physical screen resolutions
	float GetTextureScale(); // Get scale ratio between virtual and texture resolutions
	glm::vec2 VirtualToScreen(glm::vec2 pos); // Convert coorditates from virtual resolution to physical screen resolution
	glm::vec2 ScreenToVirtual(glm::vec2 pos); // Convert coorditates from physical screen resolution to virtual resolution
	LUNAScaleMode GetScaleMode(); // Get scale mode

	// Get height value for given resolution suffix
	// Return -1 if resolution not found
	static int GetHeightForResolution(const std::string& resolutionSuffix);
};

}
