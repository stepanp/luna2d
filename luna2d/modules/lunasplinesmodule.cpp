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

#include "lunasplinesmodule.h"

using namespace luna2d;

void LUNASplinesModule::Load(LuaScript *lua)
{
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblSplines(lua);

	tblSplines.SetField("quadraticBSpline", LuaFunction(lua, this, &LUNASplinesModule::QuadraticBSpline));

	tblLuna.SetField("splines", tblSplines);
}

// Quadratic B-Spline
// "p0" - i-1 point
// "p1" - i point
// "p2" - i+1 point
// "t" - time from 0.0 to 1.0
glm::vec2 LUNASplinesModule::QuadraticBSpline(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t)
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

