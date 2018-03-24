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

#include "lunaradialmesh.h"
#include "lunaengine.h"
#include "lunagraphics.h"
#include "lunalog.h"
#include "lunamath.h"

using namespace luna2d;

LUNARadialMesh::LUNARadialMesh(const std::weak_ptr<LUNATextureRegion>& region) : region(region)
{
	if(region.expired()) LUNA_RETURN_ERR("Attemt to create radial mesh from invalid texture region");

	auto sharedRegion = region.lock();

	mesh.SetTexture(sharedRegion->GetTexture());
	size.x = sharedRegion->GetWidthPoints();
	size.y = sharedRegion->GetHeightPoints();
	u1 = sharedRegion->GetU1();
	v1 = sharedRegion->GetV1();
	u2 = sharedRegion->GetU2();
	v2 = sharedRegion->GetV2();
	dirty = true;
}

glm::vec2 LUNARadialMesh::GetPos()
{
	return pos;
}

float LUNARadialMesh::GetX()
{
	return pos.x;
}

float LUNARadialMesh::GetY()
{
	return pos.y;
}

void LUNARadialMesh::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	dirty = true;
}

void LUNARadialMesh::SetX(float x)
{
	SetPos(x, pos.y);
}

void LUNARadialMesh::SetY(float y)
{
	SetPos(pos.x, y);
}

glm::vec2 LUNARadialMesh::GetSize()
{
	return size;
}

float LUNARadialMesh::GetWidth()
{
	return size.x;
}

float LUNARadialMesh::GetHeight()
{
	return size.y;
}

void LUNARadialMesh::SetSize(float width, float height)
{
	size.x = width;
	size.y = height;
	dirty = true;
}

void LUNARadialMesh::SetWidth(float width)
{
	SetSize(width, size.y);
}

void LUNARadialMesh::SetHeight(float height)
{
	SetSize(size.x, height);
}

float LUNARadialMesh::GetScale()
{
	return scale;
}

void LUNARadialMesh::SetScale(float scale)
{
	this->scale = scale;
	dirty = true;
}

void LUNARadialMesh::SetColor(float r, float g, float b)
{
	color = LUNAColor::RgbFloat(LUNAColor::ByteToFloat(r), LUNAColor::ByteToFloat(g), LUNAColor::ByteToFloat(b), color.a);
	dirty = true;
}

LUNAColor LUNARadialMesh::GetColor()
{
	return color;
}

void LUNARadialMesh::SetAlpha(float alpha)
{
	color.a = alpha;
	dirty = true;
}

float LUNARadialMesh::GetAlpha()
{
	return color.a;
}

float LUNARadialMesh::GetProgress()
{
	return progress;
}

void LUNARadialMesh::SetProgress(float progress)
{
	if(progress < 0.0f || progress > 1.0f) LUNA_RETURN_ERR("Radial mesh progress should be in range [0.0f, 1.0f]");

	this->progress = progress;
	dirty = true;
}

void LUNARadialMesh::BuildMesh()
{
	mesh.Clear();

	if(progress <= 0.0f) return;

	const float ANGLE_STEP = 10;
	float radius = std::max(size.x, size.y) / 2.0f * scale;

	float angleBegin = beginAngle;
	float angleEnd = beginAngle + (360.0f * progress);

	for(float angle = angleBegin; angle < angleEnd; angle += ANGLE_STEP)
	{
		float remainingAngle = angleEnd - angle;
		float angleStep = (remainingAngle < ANGLE_STEP && remainingAngle > 0) ? remainingAngle : ANGLE_STEP;

		mesh.AddVertex(pos.x, pos.y, color.r, color.g, color.b, color.a, u1 + (u2 - u1) * 0.5f, v1 + (v2 - v1) * 0.5f);

		glm::vec2 pos1 = pos + (glm::rotate(glm::vec2(radius, 0), glm::radians(angle)));
		glm::vec2 pos2 = pos + (glm::rotate(glm::vec2(radius, 0), glm::radians(angle + angleStep)));

		float uFactor1 = (((pos1.x - pos.x) / radius) + 1.0f) / 2.0f;
		float vFactor1 = 1.0f - (((pos1.y - pos.y) / radius) + 1.0f) / 2.0f;
		float uFactor2 = (((pos2.x - pos.x) / radius) + 1.0f) / 2.0f;
		float vFactor2 = 1.0f - (((pos2.y - pos.y) / radius) + 1.0f) / 2.0f;

		mesh.AddVertex(pos1.x, pos1.y, color.r, color.g, color.b, color.a, u1 + (u2 - u1) * uFactor1, v1 + (v2 - v1) * vFactor1);
		mesh.AddVertex(pos2.x, pos2.y, color.r, color.g, color.b, color.a, u1 + (u2 - u1) * uFactor2, v1 + (v2 - v1) * vFactor2);
	}

	dirty = false;
}

void LUNARadialMesh::Render()
{
	if(dirty) BuildMesh();
	mesh.Render();
}
