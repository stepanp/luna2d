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
	if(needUpdateCache)
	{
		UpdateBoudingBox();
		needUpdateCache = false;
	}

	return cachedBBox;
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
	needUpdateCache = true;
}

void LUNABounds::SetY(float y)
{
	pos.y = y;
	needUpdateCache = true;
}

glm::vec2 LUNABounds::GetPos()
{
	return pos;
}

void LUNABounds::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	needUpdateCache = true;
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
	needUpdateCache = true;
}

void LUNABounds::SetOriginToCenter()
{
	const LUNARect& bbox = GetBoundingBox();
	SetOrigin(-bbox.width / 2.0f, -bbox.height / 2.0f);
}

float LUNABounds::GetScaleX()
{
	return scale.x;
}

float LUNABounds::GetScaleY()
{
	return scale.y;
}

void LUNABounds::SetScaleX(float x)
{
	scale.x = x;
	needUpdateCache = true;
}

void LUNABounds::SetScaleY(float y)
{
	scale.y = y;
	needUpdateCache = true;
}

void LUNABounds::SetScale(float scale)
{
	this->scale.x = scale;
	this->scale.y = scale;
	needUpdateCache = true;
}


LUNAAABBBounds::LUNAAABBBounds(float width, float height) :
	LUNABounds(LUNABoundsType::AABB),
	width(width), height(height)
{
}

void LUNAAABBBounds::UpdateBoudingBox()
{
	cachedBBox.x = pos.x + origin.x * scale.x;
	cachedBBox.y = pos.y + origin.y * scale.y;
	cachedBBox.width = width * scale.x;
	cachedBBox.height = height * scale.y;
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
	needUpdateCache = true;
}

float LUNAAABBBounds::SetHeight(float height)
{
	this->height = height;
	needUpdateCache = true;
}

void LUNAAABBBounds::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
	needUpdateCache = true;
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
			auto polygonBounds = std::static_pointer_cast<LUNAPolygonBounds>(bounds);
			return intersect::RectPolygion(GetBoundingBox(), polygonBounds->GetVertexes());
	}

	return false;
}


LUNAPolygonBounds::LUNAPolygonBounds(const std::vector<glm::vec2>& vertexes) :
	LUNABounds(LUNABoundsType::POLYGON)
{
	SetVertexes(vertexes);
}

void LUNAPolygonBounds::UpdateVertexes()
{
	if(!needUpdateCache) return;

	for(size_t i = 0; i < vertexes.size(); i++)
	{
		transformedVertexes[i].x = pos.x + (origin.x + vertexes[i].x) * scale.x;
		transformedVertexes[i].y = pos.y + (origin.y + vertexes[i].y) * scale.y;
	}
}

void LUNAPolygonBounds::UpdateBoudingBox()
{
	UpdateVertexes();

	float left = transformedVertexes[0].x, top = 0, right = 0, bottom = transformedVertexes[0].y;
	for(const glm::vec2& vert : transformedVertexes)
	{
		left = std::min(left, vert.x);
		top = std::max(top, vert.y);
		right = std::max(right, vert.x);
		bottom = std::min(bottom, vert.y);
	}

	cachedBBox.x = left;
	cachedBBox.y = bottom;
	cachedBBox.width = right - left;
	cachedBBox.height = top - bottom;
}

const std::vector<glm::vec2>&LUNAPolygonBounds::GetVertexes()
{
	UpdateVertexes();
	return transformedVertexes;
}

void LUNAPolygonBounds::SetVertexes(const std::vector<glm::vec2>& vertexes)
{
	this->vertexes = vertexes;
	this->transformedVertexes.resize(vertexes.size());

	needUpdateCache = true;
	UpdateVertexes();
}

float LUNAPolygonBounds::GetAngle()
{
	return angle;
}

void LUNAPolygonBounds::SetAngle(float angle)
{
	this->angle = angle;
	needUpdateCache = true;
}

bool LUNAPolygonBounds::IsIntersect(const std::shared_ptr<LUNABounds>& bounds)
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
			return intersect::RectPolygion(bounds->GetBoundingBox(), GetVertexes());
		case LUNABoundsType::POLYGON:
			auto polygonBounds = std::static_pointer_cast<LUNAPolygonBounds>(bounds);
			return intersect::Polygions(GetVertexes(), polygonBounds->GetVertexes());
	}
}
