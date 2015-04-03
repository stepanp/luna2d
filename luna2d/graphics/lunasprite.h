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

#include "lunatexture.h"
#include "lunacolor.h"
#include "lunalua.h"
#include "lunavector2.h"

namespace luna2d{

class LUNASprite
{
	LUNA_USERDATA(LUNASprite)

public:
	LUNASprite(const LuaDynamicType& asset);

private:
	std::weak_ptr<LUNATexture> texture;
	float x, y;
	float originX, originY;
	float width, height;
	float scaleX, scaleY;
	float angle;
	float u1, v1, u2, v2;
	LUNAColor color;

public:
	float GetX();
	float GetY();
	void SetX(float x);
	void SetY(float y);
	glm::vec2 GetPos();
	void SetPos(float x, float y);
	float GetWidth();
	float GetHeight();
	void SetWidth(float width);
	void SetHeight(float height);
	void SetSize(float width, float height);
	float GetOriginX();
	float GetOriginY();
	void SetOriginX(float originX);
	void SetOriginY(float originY);
	void SetOrigin(float originX, float originY);
	void SetOriginToCenter(); // Set origin to center of sprite
	float GetScaleX();
	float GetScaleY();
	void SetScaleX(float scaleX);
	void SetScaleY(float scaleY);
	void SetScale(float scale);
	void Render();
	float GetAngle(); // Get rotation angle (in degrees)
	void SetAngle(float angle); // Set rotation angle (in degrees)
	void SetColor(float r, float g, float b);
	LuaTable GetColor();
	void SetAlpha(float alpha);
	float GetAlpha();
};

}
