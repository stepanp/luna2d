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
LuaTable LUNASplinesModule::QuadraticBSpline(const LuaTable& p0, const LuaTable& p1, const LuaTable& p2, float t)
{
	float p0x = p0.GetFloat("x");
	float p0y = p0.GetFloat("y");
	float p1x = p1.GetFloat("x");
	float p1y = p1.GetFloat("y");
	float p2x = p2.GetFloat("x");
	float p2y = p2.GetFloat("y");

	float x0 = (p0x - 2 * p1x + p2x) / 2.0f;
	float x1 = (-2 * p0x + 2 * p1x) / 2.0f;
	float x2 = (p0x + p1x) / 2.0f;

	float y0 = (p0y - 2 * p1y + p2y) / 2.0f;
	float y1 = (-2 * p0y + 2 * p1y) / 2.0f;
	float y2 = (p0y + p1y) / 2.0f;

	float x = t * t * x0 + t * x1 + x2;
	float y = t * t * y0 + t * y1 + y2;

	LuaTable tblRet(LUNAEngine::SharedLua());
	tblRet.SetField("x", x);
	tblRet.SetField("y", y);
	return tblRet;
}

