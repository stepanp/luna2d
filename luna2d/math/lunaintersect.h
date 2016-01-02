//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

//--------------------------------------------------
// Functions for check intersections between objects
//--------------------------------------------------
namespace luna2d{ namespace intersect{

bool PointInRectangle(const glm::vec2& point, const LUNARect& rect); // Check for point insinde in rectangle
bool PointInCircle(const glm::vec2& point, const glm::vec2& circleCenter, float r); // Check for point insinde in cirle
bool PointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon); // Check for point inside polygon
bool Rectangles(const LUNARect& rect1, const LUNARect& rect2); // Check intersection between two rectangles
bool RectPolygion(const LUNARect rect, const std::vector<glm::vec2>& polygon); // Check intersection between rect and polygion
bool Polygions(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2); // Check intersection between two polygions
bool Lines(const LuaTable& line1, const LuaTable& line2); // Check intersection between two lines
bool LineCircle(const LuaTable& line, const glm::vec2& point, float r); // Check intersection between line and circle
LuaTable PointBetweenLines(const LuaTable& line1, const LuaTable& line2); // Get intersection point between two lines

}}
