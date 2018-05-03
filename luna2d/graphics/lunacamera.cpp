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

#include "lunacamera.h"
#include "lunasizes.h"
#include "lunagraphics.h"

using namespace luna2d;

LUNACamera::LUNACamera(float width, int height) :
	width(width),
	height(height),
	zoom(1.0f)
{
	UpdateMatrix();
}

void LUNACamera::UpdateMatrix()
{
	float halfWidth = (width * zoom) / 2.0f;
	float halfHeight = (height * zoom) / 2.0f;

	matrix = glm::ortho(pos.x - halfWidth, pos.x + halfWidth, pos.y - halfHeight, pos.y + halfHeight);
}

void LUNACamera::UpdateRender()
{
	// Make render call if camera has been changed during render
	LUNARenderer* renderer = LUNAEngine::SharedGraphics()->GetRenderer();
	if(renderer && renderer->IsInProgress()) renderer->Render();
}

float LUNACamera::GetX()
{
	return pos.x;
}

float LUNACamera::GetY()
{
	return pos.y;
}

void LUNACamera::SetX(float x)
{
	SetPos(x, pos.y);
}

void LUNACamera::SetY(float y)
{
	SetPos(pos.x, y);
}

const glm::vec2& LUNACamera::GetPos()
{
	return pos;
}

void LUNACamera::SetPos(float x, float y)
{
	UpdateRender();

	pos.x = x;
	pos.y = y;

	UpdateMatrix();
}

float LUNACamera::GetZoom()
{
	return zoom;
}

void LUNACamera::SetZoom(float zoom)
{
	UpdateRender();

	this->zoom = 1.0f / zoom;
	UpdateMatrix();
}

const glm::mat4& LUNACamera::GetMatrix()
{
	return matrix;
}

// Convert coordinates from camera to physical screen
glm::vec2 LUNACamera::Project(const glm::vec2& pos)
{
	auto sizes = LUNAEngine::SharedSizes();

	glm::vec3 transformedPos = glm::project(glm::vec3(pos.x, pos.y, 0.0f), glm::mat4(1.0f), matrix,
		glm::vec4(0, 0, sizes->GetPhysicalScreenWidth(), sizes->GetPhysicalScreenHeight()));

	return glm::vec2(transformedPos.x, transformedPos.y);
}

// Convert coordinates from physical screen to camera
glm::vec2 LUNACamera::Unproject(const glm::vec2& pos)
{
	auto sizes = LUNAEngine::SharedSizes();

	glm::vec3 transformedPos = glm::unProject(glm::vec3(pos.x, pos.y, 0.0f), glm::mat4(1.0f), matrix,
		glm::vec4(0, 0, sizes->GetPhysicalScreenWidth(), sizes->GetPhysicalScreenHeight()));

	return glm::vec2(transformedPos.x, transformedPos.y);
}
