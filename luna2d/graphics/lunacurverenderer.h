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
#include "lunasplinesmodule.h"

namespace luna2d{

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
	LUNASplinesModule* splines;

	//--------------
	// Curve params:
	//--------------
	int textureId; // Texture id
	float u1,v1,u2,v2; // Texture coordinates for curve
	float width; // Curve width

private:
	void BuildMesh(); // Build mesh from curve knots

public:
	void SetKnots(const std::vector<glm::vec2>& knots); // Set curve knots (points)
	void Render();
};

}
