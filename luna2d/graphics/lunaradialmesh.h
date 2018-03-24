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
#include "lunatextureregion.h"

namespace luna2d{

class LUNARadialMesh
{
	LUNA_USERDATA(LUNARadialMesh)

public:
	LUNARadialMesh(const std::weak_ptr<LUNATextureRegion>& region);

private:
	LUNAMesh mesh;
	std::weak_ptr<LUNATextureRegion> region;
	glm::vec2 pos;
	glm::vec2 size;
	float scale = 1.0f;
	float u1 = 0.0f;
	float v1 = 0.0f;
	float u2 = 0.0f;
	float v2 = 0.0f;
	LUNAColor color = LUNAColor::WHITE;
	float beginAngle = 90;
	float progress = 1.0f;
	bool dirty = false;

private:
	void BuildMesh();

public:
	glm::vec2 GetPos();
	float GetX();
	float GetY();
	void SetPos(float x, float y);
	void SetX(float x);
	void SetY(float y);
	glm::vec2 GetSize();
	float GetWidth();
	float GetHeight();
	void SetSize(float width, float height);
	void SetWidth(float width);
	void SetHeight(float height);
	float GetScale();
	void SetScale(float scale);
	void SetColor(float r, float g, float b);
	LUNAColor GetColor();
	void SetAlpha(float alpha);
	float GetAlpha();
	float GetProgress();
	void SetProgress(float progress);
	void Render();
};

}
