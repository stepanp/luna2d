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

#include "lunaglm.h"
#include "lunalua.h"

namespace luna2d{

//--------------------
// Orthographic camera
//--------------------
class LUNACamera
{
	LUNA_USERDATA(LUNACamera)

public:
	LUNACamera(float width, int height);

private:
	float width, height;
	float zoom;
	glm::vec2 pos;
	glm::mat4 matrix;

private:
	void UpdateMatrix();
	void UpdateRender();

public:
	float GetX();
	float GetY();
	void SetX(float x);
	void SetY(float y);
	const glm::vec2& GetPos();
	void SetPos(float x, float y);
	float GetZoom();
	void SetZoom(float zoom);
	const glm::mat4& GetMatrix();

	// Convert coordinates from camera to physical screen
	glm::vec2 Project(const glm::vec2& pos);

	// Convert coordinates from physical screen to camera
	glm::vec2 Unproject(const glm::vec2& pos);
};

}
