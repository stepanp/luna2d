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

#include "lunabounds.h"
#include "lunaintersect.h"

using namespace luna2d;

LUNABounds::LUNABounds(LUNABoundsType type) :
	type(type)
{
}

LUNABoundsType LUNABounds::GetType()
{
	return type;
}

const LUNARect& LUNABounds::GetBoundingBox()
{
	if(needUpdateBBox)
	{
		UpdateBoudingBox();
		needUpdateBBox = false;
	}

	return bbox;
}

float LUNABounds::GetX()
{
	return pos.x;
}

float LUNABounds::GetY()
{
	return pos.y;
}

void LUNABounds::SetX(float x)
{
	pos.x = x;
	needUpdateBBox = true;
}

void LUNABounds::SetY(float y)
{
	pos.y = y;
	needUpdateBBox = true;
}

glm::vec2 LUNABounds::GetPos()
{
	return pos;
}

void LUNABounds::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	needUpdateBBox = true;
}

float LUNABounds::GetOriginX()
{
	return origin.x;
}

float LUNABounds::GetOriginY()
{
	return origin.y;
}

void LUNABounds::SetOriginX(float originX)
{
	origin.x = originX;
}

void LUNABounds::SetOriginY(float originY)
{
	origin.y = originY;
}

glm::vec2 LUNABounds::GetOrigin()
{
	return origin;
}

void LUNABounds::SetOrigin(float originX, float originY)
{
	origin.x = originX;
	origin.y = originY;
	needUpdateBBox = true;
}

void LUNABounds::SetOriginToCenter()
{
	const LUNARect& bbox = GetBoundingBox();
	SetOrigin(-bbox.width / 2.0f, -bbox.height / 2.0f);
}


LUNAAABBBounds::LUNAAABBBounds(float width, float height) :
	LUNABounds(LUNABoundsType::AABB),
	width(width), height(height)
{
}

void LUNAAABBBounds::UpdateBoudingBox()
{
	bbox.x = pos.x + origin.x;
	bbox.y = pos.y + origin.y;
	bbox.width = width;
	bbox.height = height;
}

float LUNAAABBBounds::GetWidth()
{
	return width;
}

float LUNAAABBBounds::GetHeight()
{
	return height;
}

void LUNAAABBBounds::SetWidth(float width)
{
	this->width = width;
	needUpdateBBox = true;
}

float LUNAAABBBounds::SetHeight(float height)
{
	this->height = height;
	needUpdateBBox = true;
}

void LUNAAABBBounds::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
	needUpdateBBox = true;
}

bool LUNAAABBBounds::IsIntersect(const std::shared_ptr<LUNABounds>& bounds)
{
	if(!bounds)
	{
		LUNA_LOGE("Attemt check intersection with invalid bounds");
		return false;
	}

	bool bboxIntersects = intersect::Rectangles(GetBoundingBox(), bounds->GetBoundingBox());
	if(!bboxIntersects) return false;

	switch(bounds->GetType())
	{
		case LUNABoundsType::AABB:
			return bboxIntersects;
		case LUNABoundsType::POLYGON:
			return false;
	}

	return false;
}
