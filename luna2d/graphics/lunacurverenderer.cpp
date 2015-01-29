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

#include "lunacurverenderer.h"
#include "lunasplinesmodule.h"

using namespace luna2d;

LUNACurveRenderer::LUNACurveRenderer(const LuaTable& params)
{
	textureId = params.GetInt("texture");
	u1 = params.GetFloat("u1");
	v1 = params.GetFloat("v1");
	u2 = params.GetFloat("u2");
	v2 = params.GetFloat("v2");
	width = params.GetFloat("width");

	mesh = std::unique_ptr<LUNAMesh>(new LUNAMesh(textureId));
	splines = LUNAEngine::SharedModule<LUNASplinesModule>("splines");
}

// Build mesh from curve knots
void LUNACurveRenderer::BuildMesh()
{
	mesh->Clear();



}

// Set curve knots (points)
void LUNACurveRenderer::SetKnots(const std::vector<glm::vec2>& knots)
{
	this->knots.clear();
	this->knots.insert(this->knots.begin(), knots.begin(), knots.end());
	BuildMesh();
}

void LUNACurveRenderer::Render()
{
	mesh->Render();
}

