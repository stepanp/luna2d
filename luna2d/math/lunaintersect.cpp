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
#include "lunaintersect.h"

using namespace luna2d;
using namespace luna2d::intersect;

static const glm::vec2 NAN_VEC2 = glm::vec2(std::nanf(""), std::nanf(""));

// Check for point insinde in rectangle
bool luna2d::intersect::PointInRectangle(const glm::vec2& point, const LUNARect& rect)
{
	return point.x > rect.x && point.y > rect.y &&
		point.x < rect.x + rect.width && point.y < rect.y + rect.height;
}

// Check for point insinde in cirle
bool luna2d::intersect::PointInCircle(const glm::vec2& point, const glm::vec2& circleCenter, float r)
{
	return glm::distance2(point, circleCenter) <= r * r;
}

// Check for point inside polygon
bool luna2d::intersect::PointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon)
{
	bool ret = false;
	int count = polygon.size();
	for(int i = 0, j = count - 1; i < count; j = i++)
	{
		if((polygon[i].y >= point.y) != (polygon[j].y >= point.y) &&
			(point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
			(polygon[j].y - polygon[i].y) + polygon[i].x)) ret = !ret;
	}

	return ret;
}

// Check intersection between two rectangles
bool luna2d::intersect::Rectangles(const LUNARect& rect1, const LUNARect& rect2)
{
	return rect1.x < rect2.x + rect2.width && rect1.x + rect1.width > rect2.x
		&& rect1.y < rect2.y + rect2.height && rect1.y + rect1.height > rect2.y;
}

// Check intersection between two lines
bool luna2d::intersect::Lines(const glm::vec2& lineBegin1, const glm::vec2& lineEnd1,
	const glm::vec2& lineBegin2, const glm::vec2& lineEnd2)
{
	float x1 = lineBegin1.x;
	float y1 = lineBegin1.y;
	float x2 = lineEnd1.x;
	float y2 = lineEnd1.y;

	float x3 = lineBegin2.x;
	float y3 = lineBegin2.y;
	float x4 = lineEnd2.x;
	float y4 = lineEnd2.y;

	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if(d == 0) return false;

	float yd = y1 - y3;
	float xd = x1 - x3;
	float ua = ((x4 - x3) * yd  - (y4 - y3) * xd) / d;
	if(ua < 0 || ua > 1) return false;

	float ub = ((x2 - x1) * yd  - (y2 - y1) * xd) / d;
	if(ub < 0 || ub > 1) return false;

	return true;
}

// Check intersection between line and circle
bool luna2d::intersect::LineCircle(const glm::vec2& lineBegin, const glm::vec2& lineEnd,
	const glm::vec2& circleCenter, float circleRadius)
{
	float x1 = lineBegin.x;
	float y1 = lineBegin.y;
	float x2 = lineEnd.x;
	float y2 = lineEnd.y;
	float cx = circleCenter.x;
	float cy = circleCenter.y;

	x1 -= cx;
	y1 -= cy;
	x2 -= cx;
	y2 -= cy;

	float dx = x2 - x1;
	float dy = y2 - y1;

	float a = dx * dx + dy * dy;
	float b = 2 * (x1 * dx + y1 * dy);
	float c = x1 * x1 + y1 * y1 - circleRadius * circleRadius;

	if(-b < 0) return c < 0;
	else if(-b < 2 * a) return (4 * a * c - b * b) < 0;

	return (a + b + c) < 0;
}

// Get intersection point between two lines
glm::vec2 luna2d::intersect::PointBetweenLines(const glm::vec2& lineBegin1, const glm::vec2& lineEnd1,
	const glm::vec2& lineBegin2, const glm::vec2& lineEnd2)
{
	float x1 = lineBegin1.x;
	float y1 = lineBegin1.y;
	float x2 = lineEnd1.x;
	float y2 = lineEnd1.y;

	float x3 = lineBegin2.x;
	float y3 = lineBegin2.y;
	float x4 = lineEnd2.x;
	float y4 = lineEnd2.y;

	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if(d == 0) return NAN_VEC2;

	float yd = y1 - y3;
	float xd = x1 - x3;
	float ua = ((x4 - x3) * yd  - (y4 - y3) * xd) / d;
	if(ua < 0 || ua > 1) return NAN_VEC2;

	float ub = ((x2 - x1) * yd  - (y2 - y1) * xd) / d;
	if(ub < 0 || ub > 1) return NAN_VEC2;

	float x = x1 + ua * (x2 - x1);
	float y = y1 + ua * (y2 - y1);

	return glm::vec2(x, y);
}

static std::vector<glm::vec2> rectPolygonCache =
{
	glm::vec2(),
	glm::vec2(),
	glm::vec2(),
	glm::vec2()
};

 // Check intersection between rect and polygion
bool luna2d::intersect::RectPolygion(const LUNARect rect, const std::vector<glm::vec2>& polygon)
{
	// Convert rect to polygoon with four vertexes
	// To avoid create new vertex array on heap, cache it to variable
	rectPolygonCache[0].x = rect.x;
	rectPolygonCache[0].y = rect.y;
	rectPolygonCache[1].x = rect.x;
	rectPolygonCache[1].y = rect.y + rect.height;
	rectPolygonCache[2].x = rect.x + rect.width;
	rectPolygonCache[2].y = rect.y + rect.height;
	rectPolygonCache[3].x = rect.x + rect.width;
	rectPolygonCache[3].y = rect.y;

	return Polygions(rectPolygonCache, polygon);
}

// Check intersection between two polygions
bool luna2d::intersect::Polygions(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2)
{
	for(const auto& vec : polygon1) if(PointInPolygon(vec, polygon2)) return true;
	for(const auto& vec : polygon2) if(PointInPolygon(vec, polygon1)) return true;

	return false;
}

// Check intersection between two circles
bool luna2d::intersect::Circles(const glm::vec2& center1, float radius1, const glm::vec2& center2, float radius2)
{
	return glm::distance2(center1, center2) <= std::powf(radius1 + radius2, 2);
}

// Check intersection between circle and polygon
bool luna2d::intersect::CirclePolygon(const glm::vec2& center, float radius, const std::vector<glm::vec2>& polygon)
{
	LUNA_LOGE("Function luna2d::intersect::CirclePolygon is not implemented");
	return false;
}

// Check intersection between circle and rectangle
bool luna2d::intersect::CircleRect(const glm::vec2& center, float radius, const LUNARect& rect)
{
	glm::vec2 rectCenter(rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f);
	glm::vec2 rectHalfSizes(rect.width / 2.0f, rect.height / 2.0f);

	glm::vec2 difference = center - rectCenter;
	glm::vec2 clamped = glm::clamp(difference, -rectHalfSizes, rectHalfSizes);

	glm::vec2 closest = rectCenter + clamped;
	difference = closest - center;

	return glm::length2(difference) < radius * radius;
}
