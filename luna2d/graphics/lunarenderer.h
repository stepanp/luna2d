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

#include "lunacolor.h"
#include "lunashader.h"
#include "lunacamera.h"
#include "lunamaterial.h"

// Default shaders
#include "shaders/default.vert.h"
#include "shaders/default.frag.h"
#include "shaders/primitives.vert.h"
#include "shaders/primitives.frag.h"
#include "shaders/font.vert.h"
#include "shaders/font.frag.h"

const int RENDER_RESERVE_BATCH = 1000; // Count of polygons for which allocated memory when renderer initializing
const int RENDER_ELEMENT_PER_VERTEX = 8; // Count of array elements for each vertex

namespace luna2d{

class LUNARenderer
{
public:
	LUNARenderer();

private:
	// Vertex array for batching
	std::vector<float> vertexBatch;

	// Default shader
	std::shared_ptr<LUNAShader> defaultShader, primitivesShader, fontShader;

	// Background color
	LUNAColor backColor = LUNAColor::WHITE;

	// Camera for current render call
	std::shared_ptr<LUNACamera> camera;

	// Material for current render call
	const LUNAMaterial* curMaterial = nullptr;

	// Info for stats
	int renderCalls = 0; // Count of render calls on current frame
	int renderedVertexes = 0; // Count of rendered vertexes on current frame

	bool inProgress = false;
	bool debugRender = false;

private:
	void SetVertex(float u, float v, float x, float y, const LUNAColor& color);

public:
	bool IsInProgress();

	int GetRenderCalls();
	int GetRenderedVertexes();

	std::shared_ptr<LUNAShader> GetDefaultShader();
	std::shared_ptr<LUNAShader> GetPrimitvesShader();
	std::shared_ptr<LUNAShader> GetFontShader();

	LUNAColor GetBackgroundColor();
	void SetBackgroundColor(const LUNAColor& backColor);

	void SetCamera(const std::shared_ptr<LUNACamera>& camera);

	void EnableScissor(float x, float y, float width, float height);
	void DisableScissor();

	bool IsEnabledDebugRender();
	void EnableDebugRender(bool enable);

	void RenderQuad(float x1, float y1, float u1, float v1,
		float x2, float y2, float u2, float v2,
		float x3, float y3, float u3, float v3,
		float x4, float y4, float u4, float v4,
		const LUNAMaterial* material, const LUNAColor& color);

	void RenderVertexArray(std::vector<float>& vertexes, const LUNAMaterial* material);

	void RenderLine(float x1, float y1, float x2, float y2, const LUNAColor& color);

	void BeginRender();
	void Render();
	void EndRender();

// Reload default shaders when application lost OpenGL context
// SEE: "lunaassets.h"
#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
public:
	inline void ReloadDefaultShaders()
	{
		defaultShader->Reload(DEFAULT_VERT_SHADER, DEFAULT_FRAG_SHADER);
		primitivesShader->Reload(PRIMITIVES_VERT_SHADER, PRIMITIVES_FRAG_SHADER);
		fontShader->Reload(FONT_VERT_SHADER, FONT_FRAG_SHADER);
	}
#endif
};

}
