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

#include "lunamesh.h"
#include "lunavector2.h"

namespace luna2d{

enum class LUNATextureMappingMode
{
	STRETCH,
	REPEAT
};

const LUNAStringEnum<LUNATextureMappingMode> TEXTURE_MAPPING_MODE =
{
	"stretch",
	"repeat"
};

class LUNATextureRegion;

struct LUNACurveParams
{
	// Texture source for common curve segments
	std::weak_ptr<LUNATextureRegion> textureRegion;

	// Texture source for head curve segment
	std::weak_ptr<LUNATextureRegion> textureRegionHead;

	// Texture source for tail curve segment
	std::weak_ptr<LUNATextureRegion> textureRegionTail;

	// Mode of mapping texture to curve
	LUNATextureMappingMode textureMappingMode = LUNATextureMappingMode::STRETCH;

	// Rotate texture on 90 degree
	bool vertical = false;

	// Curve width
	float width = 1.0f;

	// Repeat texture each N segments (Only for REPEAT texture mapping mode)
	int segmentsToRepeat = 1;
};


//-------------------------------------
// Helper for rendering textured curves
//-------------------------------------
class LUNACurve
{
	LUNA_USERDATA(LUNACurve)

public:
	LUNACurve(const LUNACurveParams& params);

private:
	LUNACurveParams params;
	std::unique_ptr<LUNAMesh> mesh;
	std::vector<glm::vec2> knots;
	bool needBuild = false;

	std::function<glm::vec2(float,float, float)> getLt, getLb, getRt, getRb;

	LUNAColor color = LUNAColor::WHITE;
	float smoothFactor = 0.5f;

private:
	void SetupReadFunctions();
	void Build(); // Build curve mesh by knots

public:
	void SetParams(const LUNACurveParams& params);
	void ClearKnots();
	int GetKnotsCount();
	const std::vector<glm::vec2>& GetKnots();
	void SetKnots(const std::vector<glm::vec2>& knots);
	void AddKnot(float x, float y);
	void RemoveKnot(int index);
	void SetKnot(int index, float x, float y);
	LUNAColor GetColor();
	void SetColor(float r, float g, float b);
	float GetAlpha();
	void SetAlpha(float alpha);
	float GetSmoothFactor();
	void SetSmoothFactor(float factor);
	void Render();
};


template<>
struct LuaStack<LUNACurveParams>
{
	static void Push(lua_State* luaVm, const LUNACurveParams& params);
	static LUNACurveParams Pop(lua_State* luaVm, int index = -1);
};

}
