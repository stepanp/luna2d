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

#pragma once

#include "lunashader.h"
#include "lunatextureregion.h"
#include "lunacolor.h"
#include "lunalua.h"
#include <vector>

// Default shaders
#include "shaders/default.vert.h"
#include "shaders/default.frag.h"
#include "shaders/primitives.vert.h"
#include "shaders/primitives.frag.h"

#define LUNA_MAX_POLYGONS 1000 // Max polygons count in batcher
#define LUNA_ELEMENT_PER_VERTEX 8 // Count of array elements for each vertex

namespace luna2d{

class LUNARenderer
{
public:
	LUNARenderer();
	~LUNARenderer();

private:
	std::vector<float> vertexBatch; // Vertex array for batching
	glm::mat4 matrix; // Transformation matrix

	LUNAColor backColor; // Background color
	LUNAShader *shader;
	LUNAShader* primitivesShader;
	LUNATexture *curTexture; // Texture program for current render call

	// Stats
	int renderCalls; // Count of render calls on current frame
	bool inProgress;
	bool debugRender;

private:
	void SetVertex(float u, float v, float x, float y, const LUNAColor& color);

public:
	bool IsInProgress();
	void ReloadDefaultShader();
	int GetRenderCalls();

	void SetBackgroundColor(const LUNAColor& backColor);

	bool IsEnabledDebugRender();
	void EnableDebugRender(bool enable);

	void RenderQuad(LUNATexture *texture,
		float x1, float y1, float u1, float v1,
		float x2, float y2, float u2, float v2,
		float x3, float y3, float u3, float v3,
		float x4, float y4, float u4, float v4,
		const LUNAColor &color);
	void RenderVertexArray(LUNATexture* texture, const std::vector<float>& vertexes);

	// Render primitives
	void RenderLine(float x1, float y1, float x2, float y2, const LUNAColor &color);

	void Render();
	void Begin();
	void End();
};

}
