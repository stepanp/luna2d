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

#include "lunarenderer.h"
#include "lunasizes.h"
#include "lunalog.h"
#include "lunaassets.h"

using namespace luna2d;

LUNARenderer::LUNARenderer()
{
	// Initialize batch vertex array
	vertexBatch.reserve(RENDER_RESERVE_BATCH * RENDER_ELEMENT_PER_VERTEX);

	// Initialize default shaders
	defaultShader = std::make_shared<LUNAShader>(LUNA_DEFAULT_VERT_SHADER, LUNA_DEFAULT_FRAG_SHADER);
	primitivesShader = std::make_shared<LUNAShader>(LUNA_PRIMITIVES_VERT_SHADER, LUNA_PRIMITIVES_FRAG_SHADER);
	fontShader = std::make_shared<LUNAShader>(LUNA_FONT_VERT_SHADER, LUNA_FONT_FRAG_SHADER);

	glViewport(0, 0, LUNAEngine::SharedSizes()->GetPhysicalScreenWidth(), LUNAEngine::SharedSizes()->GetPhysicalScreenHeight());
}

void LUNARenderer::SetVertex(float u, float v, float x, float y, const LUNAColor& color)
{
	// Position
	vertexBatch.push_back(x);
	vertexBatch.push_back(y);

	// Color
	vertexBatch.push_back(color.r);
	vertexBatch.push_back(color.g);
	vertexBatch.push_back(color.b);
	vertexBatch.push_back(color.a);

	// Texture coordinates
	vertexBatch.push_back(u);
	vertexBatch.push_back(v);
}

bool LUNARenderer::IsInProgress()
{
	return inProgress;
}

int LUNARenderer::GetRenderCalls()
{
	return renderCalls;
}

int LUNARenderer::GetRenderedVertexes()
{
	return renderedVertexes;
}

std::shared_ptr<LUNAShader> LUNARenderer::GetDefaultShader()
{
	return defaultShader;
}

std::shared_ptr<LUNAShader> LUNARenderer::GetPrimitvesShader()
{
	return primitivesShader;
}

std::shared_ptr<LUNAShader> LUNARenderer::GetFontShader()
{
	return fontShader;
}

LUNAColor LUNARenderer::GetBackgroundColor()
{
	return backColor;
}

void LUNARenderer::SetBackgroundColor(const LUNAColor& backColor)
{
	this->backColor = backColor;
}

void LUNARenderer::SetCamera(const std::shared_ptr<LUNACamera>& camera)
{
	this->camera = camera;
}

void LUNARenderer::EnableScissor(float x, float y, float width, float height)
{
	Render();

	auto sizes = LUNAEngine::SharedSizes();
	auto pos = sizes->VirtualToScreen(glm::vec2(x, y));
	auto size = sizes->VirtualToScreen(glm::vec2(x + width, y + height));
	size -= pos;

	glEnable(GL_SCISSOR_TEST);
	glScissor((GLint)std::roundf(pos.x), (GLint)std::roundf(pos.y),
		(GLsizei)std::roundf(size.x), (GLsizei)std::roundf(size.y));
}

void LUNARenderer::DisableScissor()
{
	Render();

	glDisable(GL_SCISSOR_TEST);
}

bool LUNARenderer::IsEnabledDebugRender()
{
	return debugRender;
}

void LUNARenderer::EnableDebugRender(bool enable)
{
	debugRender = enable;
}

void LUNARenderer::RenderQuad(
	float x1, float y1, float u1, float v1,
	float x2, float y2, float u2, float v2,
	float x3, float y3, float u3, float v3,
	float x4, float y4, float u4, float v4,
	const LUNAMaterial* material, const LUNAColor& color)
{
	if(curMaterial && *curMaterial != *material) Render();
	curMaterial = material;

	// Make quad from two triangles like:
	// 2-3 5
	// |/ /|
	// 1 4-6

	SetVertex(u1, v1, x1, y1, color); // 1
	SetVertex(u2, v2, x2, y2, color); // 2
	SetVertex(u3, v3, x3, y3, color); // 3

	SetVertex(u1, v1, x1, y1, color); // 4
	SetVertex(u3, v3, x3, y3, color); // 5
	SetVertex(u4, v4, x4, y4, color); // 6

	if(debugRender)
	{
		RenderLine(x1, y1, x2, y2, color); // 1-2
		RenderLine(x1, y1, x3, y3, color); // 1-3
		RenderLine(x2, y2, x3, y3, color); // 2-3
		RenderLine(x1, y1, x4, y4, color); // 4-6
		RenderLine(x3, y3, x4, y4, color); // 5-6
	}
}

void LUNARenderer::RenderVertexArray(std::vector<float>& vertexes, const LUNAMaterial* material)
{
	if(curMaterial && *curMaterial != *material) Render();
	curMaterial = material;

	vertexBatch.insert(vertexBatch.end(), vertexes.begin(), vertexes.end());

	if(debugRender)
	{
		int count = vertexes.size();
		for(int i = 0; i < count; i += RENDER_ELEMENT_PER_VERTEX * 3)
		{
			float x1 = vertexes[i];
			float y1 = vertexes[i + 1];
			float x2 = vertexes[i + RENDER_ELEMENT_PER_VERTEX];
			float y2 = vertexes[i + RENDER_ELEMENT_PER_VERTEX + 1];
			float x3 = vertexes[i + RENDER_ELEMENT_PER_VERTEX * 2];
			float y3 = vertexes[i + RENDER_ELEMENT_PER_VERTEX * 2 + 1];

			RenderLine(x1, y1, x2, y2, LUNAColor::WHITE);
			RenderLine(x1, y1, x3, y3, LUNAColor::WHITE);
			RenderLine(x2, y2, x3, y3, LUNAColor::WHITE);
		}
	}
}

void LUNARenderer::RenderLine(float x1, float y1, float x2, float y2, const LUNAColor& color)
{
	Render();
	curMaterial = nullptr;

	// Position
	float vertexes[] =
	{
		x1, y1,
		color.r, color.g, color.b, color.a,
		0, 0, // Unused texture coords
		x2, y2,
		color.r, color.g, color.b, color.a,
		0, 0, // Unused texture coords
	};

	primitivesShader->Bind();
	primitivesShader->SetPositionAttribute(vertexes);
	primitivesShader->SetColorAttribute(vertexes);
	primitivesShader->SetTransformMatrix(camera->GetMatrix());
	glDrawArrays(GL_LINES, 0, 2);
}

void LUNARenderer::BeginRender()
{
	inProgress = true;
	renderCalls = 0;
	renderedVertexes = 0;

	vertexBatch.clear();

	glDisable(GL_DEPTH_TEST); // Depth test not needed for 2D

	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void LUNARenderer::Render()
{
	if(vertexBatch.empty()) return;

	int vertexCount = vertexBatch.size() / RENDER_ELEMENT_PER_VERTEX;

	// Set blending mode
	switch(curMaterial->blending)
	{
	case LUNABlendingMode::NONE:
		glDisable(GL_BLEND);
		break;
	case LUNABlendingMode::ALPHA:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case LUNABlendingMode::ADDITIVE:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	}

	auto shader = curMaterial->shader.lock();
	auto texture = curMaterial->texture.lock();

	shader->Bind();
	shader->SetPositionAttribute(vertexBatch.data());
	shader->SetColorAttribute(vertexBatch.data());
	shader->SetTexCoordsAttribute(vertexBatch.data());
	shader->SetTransformMatrix(camera->GetMatrix());
	shader->SetTextureUniform(texture.get());

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	vertexBatch.clear();
	renderedVertexes += vertexCount;
	renderCalls++;

	LUNA_CHECK_GL_ERROR();
}

void LUNARenderer::EndRender()
{
	Render();

	curMaterial = nullptr;
	inProgress = false;
}
