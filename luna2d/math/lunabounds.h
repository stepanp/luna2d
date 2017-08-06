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

#include "lunavector2.h"
#include "lunarect.h"

namespace luna2d{

enum class LUNABoundsType
{
	AABB,
	CIRCLE,
	POLYGON
};


class LUNABounds
{
	LUNA_USERDATA(LUNABounds)

protected:
	LUNABounds(LUNABoundsType type);

protected:
	LUNABoundsType type;
	glm::vec2 pos;
	glm::vec2 origin;
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	LUNARect cachedBBox;
	bool needUpdateCache = true;

protected:
	virtual void UpdateBoudingBox() = 0;

public:
	LUNABoundsType GetType();
	const LUNARect& GetBoundingBox();
	float GetX();
	float GetY();
	void SetX(float x);
	void SetY(float y);
	glm::vec2 GetPos();
	void SetPos(float x, float y);
	float GetOriginX();
	float GetOriginY();
	void SetOriginX(float originX);
	void SetOriginY(float originY);
	glm::vec2 GetOrigin();
	void SetOrigin(float originX, float originY);
	void SetOriginToCenter();
	float GetScaleX();
	float GetScaleY();
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScale(float scale);

	virtual bool IsIntersect(const std::shared_ptr<LUNABounds>& bounds) = 0;
};


class LUNAAABBBounds : public LUNABounds
{
	LUNA_USERDATA_DERIVED(LUNABounds, LUNAAABBBounds)

public:
	LUNAAABBBounds(float width, float height);

private:
	float width;
	float height;

private:
	virtual void UpdateBoudingBox();

public:
	float GetWidth();
	float GetHeight();
	void SetWidth(float width);
	void SetHeight(float height);
	void SetSize(float width, float height);

	virtual bool IsIntersect(const std::shared_ptr<LUNABounds>& bounds);
};


class LUNACircleBounds : public LUNABounds
{
	LUNA_USERDATA_DERIVED(LUNABounds, LUNACircleBounds)

public:
	LUNACircleBounds(float radius);

private:
	float radius;

private:
	virtual void UpdateBoudingBox();

public:
	glm::vec2 GetCenter();
	float GetRadius();
	void SetRadius(float radius);

	virtual bool IsIntersect(const std::shared_ptr<LUNABounds>& bounds);
};


class LUNAPolygonBounds : public LUNABounds
{
	LUNA_USERDATA_DERIVED(LUNABounds, LUNAPolygonBounds)

public:
	LUNAPolygonBounds(const std::vector<glm::vec2>& vertexes);

private:
	std::vector<glm::vec2> vertexes, transformedVertexes;
	float angle = 0;

private:
	void UpdateVertexes();
	virtual void UpdateBoudingBox();

public:
	const std::vector<glm::vec2>& GetVertexes();
	void SetVertexes(const std::vector<glm::vec2>& vertexes);
	float GetAngle();
	void SetAngle(float angle);

	virtual bool IsIntersect(const std::shared_ptr<LUNABounds>& bounds);
};

}
