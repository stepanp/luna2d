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

#include "lunasplines.h"

using namespace luna2d;
using namespace luna2d::splines;

// Quadratic B-Spline
// "p0" - i-1 point
// "p1" - i point
// "p2" - i+1 point
// "t" - time from 0.0 to 1.0
glm::vec2 luna2d::splines::QuadraticBSpline(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t)
{
	float x0 = (p0.x - 2.0f * p1.x + p2.x) / 2.0f;
	float x1 = (-2.0f * p0.x + 2.0f * p1.x) / 2.0f;
	float x2 = (p0.x + p1.x) / 2.0f;

	float y0 = (p0.y - 2.0f * p1.y + p2.y) / 2.0f;
	float y1 = (-2.0f * p0.y + 2.0f * p1.y) / 2.0f;
	float y2 = (p0.y + p1.y) / 2.0f;

	float x = t * t * x0 + t * x1 + x2;
	float y = t * t * y0 + t * y1 + y2;

	return glm::vec2(x, y);
}

// Cubic Bezier function
glm::vec2 luna2d::splines::CubicBezier(const glm::vec2& p0, const glm::vec2& p1,
	const glm::vec2& p2, const glm::vec2& p3, float t)
{
	float x0 = p3.x + 3 * (p1.x - p2.x) - p0.x;
	float x1 = p0.x - 2 * p1.x + p2.x;
	float x2 = p1.x - p0.x;

	float y0 = p3.y + 3 * (p1.y - p2.y) - p0.y;
	float y1 = p0.y - 2 * p1.y + p2.y;
	float y2 = p1.y - p0.y;

	float x = t * t * t * x0 + 3 * t * t * x1 + 3 * t * x2 + p0.x;
	float y = t * t * t * y0 + 3 * t * t * y1 + 3 * t * y2 + p0.y;

	return glm::vec2(x, y);
}
