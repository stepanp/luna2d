//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaphysicsutils.h"
#include "lunasizes.h"

using namespace luna2d;

// Default value choosen so that screen height in Box2D world amounted to 10m
float LUNAPhysicsUtils::pixelsInMeter = BASE_SIZE / 10.0f;

void LUNAPhysicsUtils::SetPixelsInMeter(float value)
{
	pixelsInMeter = value;
}

float LUNAPhysicsUtils::PixelsToMeters(float pixels)
{
	return pixels / pixelsInMeter;
}

float LUNAPhysicsUtils::MetersToPixels(float meters)
{
	return meters * pixelsInMeter;
}

// Convert LuaTable to b2Vec2 with converting pixels to meters
b2Vec2 LUNAPhysicsUtils::TableToB2Vector(const LuaTable& table)
{
	float metersX = PixelsToMeters(table.GetFloat("x"));
	float metersY = PixelsToMeters(table.GetFloat("y"));

	return b2Vec2(metersX, metersY);
}

// Convert b2Vec2 to LuaTable with converting meters to pixels
LuaTable LUNAPhysicsUtils::B2VectorToTable(const b2Vec2& vec)
{
	LuaTable tblRet(LUNAEngine::SharedLua());

	tblRet.SetField("x", MetersToPixels(vec.x));
	tblRet.SetField("y", MetersToPixels(vec.y));

	return std::move(tblRet);
}
