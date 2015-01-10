//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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
#include "lunalua.h"
#include <cmath>
#include <cfloat>

using namespace luna2d;

LUNASizes::LUNASizes(int screenWidth, int screenHeight, LUNAConfig* config)
{
	physicalWidth = screenWidth;
	physicalHeight = screenHeight;
	scaleMode = config->scaleMode;
	baseWidth = config->baseWidth;
	baseHeight = config->baseHeight;

	// Portrait
	if(config->orientation == LUNAOrientation::PORTRAIT)
	{
		aspectRatio = physicalHeight / (float)physicalWidth;
		scaleFactor = physicalWidth / (float)BASE_SIZE;
		virtualWidth = BASE_SIZE;
		virtualHeight = (int)(BASE_SIZE * aspectRatio);
	}

	// Landscape
	else
	{
		aspectRatio = physicalWidth / (float)physicalHeight;
		scaleFactor = physicalHeight / (float)BASE_SIZE;

		if(scaleMode >= LUNAScaleMode::FIT_TO_WIDTH_TOP)
		{
			virtualWidth = baseWidth;
			virtualHeight = (int)(virtualWidth / aspectRatio);
		}
		else
		{
			virtualWidth = (int)(BASE_SIZE * aspectRatio);
			virtualHeight = BASE_SIZE;
		}
	}

	SelectResolution(config);
	textureScale = BASE_SIZE / (float)LUNASizes::GetHeightForResolution(resolutionSuffix);
	BuildTransformMatrix();

	LuaScript *lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	LuaTable tblSizes(lua);
	LuaFunction fnGetVirtualWidth(lua, this, &LUNASizes::GetVirtualScreenWidth);
	LuaFunction fnGetVirtualHeight(lua, this, &LUNASizes::GetVirtualScreenHeight);

	tblSizes.SetField("getResolutionName", LuaFunction(lua, this, &LUNASizes::GetResolutionSuffix));
	tblSizes.SetField("getAspectRatio", LuaFunction(lua, this, &LUNASizes::GetAspectRatio));
	tblSizes.SetField("getPhysicalScreenWidth", LuaFunction(lua, this, &LUNASizes::GetPhysicalScreenWidth));
	tblSizes.SetField("getPhysicalScreenHeight", LuaFunction(lua, this, &LUNASizes::GetPhysicalScreenHeight));
	tblSizes.SetField("getVirtualScreenWidth", fnGetVirtualWidth);
	tblSizes.SetField("getVirtualScreenHeight", fnGetVirtualHeight);
	tblSizes.SetField("getBaseScreenWidth", LuaFunction(lua, this, &LUNASizes::GetBaseScreenWidth));
	tblSizes.SetField("getBaseScreenHeight", LuaFunction(lua, this, &LUNASizes::GetBaseScreenHeight));

	// getScreenWidth/getScreenHeight is aliases for getVirtualScreenWidth/getVirtualScreenHeight
	tblSizes.SetField("getScreenWidth", fnGetVirtualWidth);
	tblSizes.SetField("getScreenHeight", fnGetVirtualHeight);

	tblLuna.SetField("sizes", tblSizes);
}

void LUNASizes::SelectResolution(LUNAConfig *config)
{
	int count = config->resolutions.size();
	float minDiff = FLT_MAX;
	int index = -1;

	for(int i = 0; i < count; i++)
	{
		int height = RESOLUTIONS_TABLE.at(config->resolutions[i]);
		float resScaleFactor = height / (float)BASE_SIZE;
		float diff = std::fabs(resScaleFactor - scaleFactor);

		if(diff < minDiff)
		{
			minDiff = diff;
			index = i;
		}
	}

	resolutionSuffix = config->resolutions[index];
}

// Build transformation matrix by scale mode
void LUNASizes::BuildTransformMatrix()
{
	float left = 0.0f;
	float right = 0.0f;
	float bottom = 0.0f;
	float top = 0.0f;

	// Build transformation matrix by scale mode
	switch(scaleMode)
	{
	case LUNAScaleMode::FIT_TO_HEIGHT_LEFT:
		right = virtualWidth;
		top = virtualHeight;
		break;
	case LUNAScaleMode::FIT_TO_HEIGHT_RIGHT:
		{
			float leftOffset = virtualWidth - baseWidth;
			left = -leftOffset;
			right = virtualWidth - leftOffset;
			top = virtualHeight;
		}
		break;
	case LUNAScaleMode::FIT_TO_HEIGHT_CENTER:
		{
			float leftOffset = (virtualWidth - baseWidth) / 2;
			left = -leftOffset;
			right = virtualWidth - leftOffset;
			top = virtualHeight;
		}
		break;
	case LUNAScaleMode::FIT_TO_WIDTH_TOP:
		{
			float bottomOffset = baseHeight - virtualHeight;
			bottom = bottomOffset;
			right = virtualWidth;
			top = virtualHeight + bottomOffset;
		}
		break;
	case LUNAScaleMode::FIT_TO_WIDTH_BOTTOM:
		right = virtualWidth;
		top = virtualHeight;
		break;
	case LUNAScaleMode::FIT_TO_WIDTH_CENTER:
		{
			float bottomOffset = (baseHeight - virtualHeight) / 2;
			bottom = bottomOffset;
			right = virtualWidth;
			top = virtualHeight + bottomOffset;
		}
		break;
	}

	transformMatrix = glm::ortho(left, right, bottom, top);
}

// Get transformation matrix for renderer
const glm::mat4& LUNASizes::GetTransformMatrix()
{
	return transformMatrix;
}

std::string LUNASizes::GetResolutionSuffix()
{
	return resolutionSuffix;
}

// Get physical screen width
int LUNASizes::GetPhysicalScreenWidth()
{
	return physicalWidth;
}

// Get physical screen height
int LUNASizes::GetPhysicalScreenHeight()
{
	return physicalHeight;
}

// Get virtual screen width
int LUNASizes::GetVirtualScreenWidth()
{
	return virtualWidth;
}

// Get virtual screen height
int LUNASizes::GetVirtualScreenHeight()
{
	return virtualHeight;
}

// Get base screen width
int LUNASizes::GetBaseScreenWidth()
{
	return baseWidth;
}

// Get base screen height
int LUNASizes::GetBaseScreenHeight()
{
	return baseHeight;
}

// Get aspect ratio
float LUNASizes::GetAspectRatio()
{
	return aspectRatio;
}

// Get scale ratio between virtual and physical resolutions
float LUNASizes::GetScaleFactor()
{
	return scaleFactor;
}

// Get scale ratio between virtual and texture resolutions
float LUNASizes::GetTextureScale()
{
	return textureScale;
}

// Convert coorditates from virtual resolution to physical screen resolution
glm::vec2 LUNASizes::VirtualToScreen(glm::vec2 pos)
{
	glm::vec4 transformedPos = transformMatrix * glm::vec4(pos.x, pos.y, 0, 0);
	return glm::vec2(transformedPos.x, transformedPos.y);
}

// Convert coorditates from physical screen resolution to virtual resolution
glm::vec2 LUNASizes::ScreenToVirtual(glm::vec2 pos)
{
	glm::vec3 transformedPos = glm::unProject(glm::vec3(pos.x, pos.y, 0.0f), glm::mat4(1.0f), transformMatrix,
		glm::vec4(0, 0, physicalWidth, physicalHeight));

	return glm::vec2(transformedPos.x, transformedPos.y);
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
