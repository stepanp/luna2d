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

#include "lunamodule.h"
#include "lunavector2.h"

namespace luna2d{

class LUNAIntersect : public LUNAModule
{
public:
	virtual void Load(LuaScript *lua);

	bool PointInRectangle(const glm::vec2& point, const LuaTable& rect); // Check for point insinde in rectangle
	bool PointInCircle(const glm::vec2& point, const glm::vec2& circleCenter, float r); // Check for point insinde in cirle
	bool PointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon); // Check for point inside polygon
	bool Rectangles(const LuaTable& rect1, const LuaTable& rect2); // Check intersection between two rectangles
	bool Lines(const LuaTable& line1, const LuaTable& line2); // Check intersection between two lines
	bool LineCircle(const LuaTable& line, const glm::vec2& point, float r); // Check intersection between line and circle
	LuaTable PointBetweenLines(const LuaTable& line1, const LuaTable& line2); // Get intersection point between two lines
};

}
