//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

const float CURVE_SMOOTH_FACTOR = 0.5f;

//-------------------------------------
// Helper for rendering textured curves
//-------------------------------------
class LUNACurveRenderer
{
	LUNA_USERDATA(LUNACurveRenderer)

public:
	LUNACurveRenderer(const LuaTable& params);

private:
	std::unique_ptr<LUNAMesh> mesh;
	std::vector<glm::vec2> knots;
	bool needBuild = false;

	//--------------
	// Curve params:
	//--------------
	std::weak_ptr<LUNATexture> texture; // Texture id
	float u1,v1,u2,v2; // Texture coordinates for curve
	bool verticalTexture; // Rotate texture on 90 degree
	float width; // Curve width
	LUNAColor color = LUNAColor::WHITE;

private:
	void Build(); // Build curve mesh by knots

public:
	void ClearKnots();
	int GetKnotsCount();
	void AddKnot(float x, float y);
	void RemoveKnot(int index);
	void SetKnot(int index, float x, float y);
	void SetKnots(const std::vector<glm::vec2>& knots);
	LUNAColor GetColor();
	void SetColor(float r, float g, float b);
	float GetAlpha();
	void SetAlpha(float alpha);
	void Render();
};

}
