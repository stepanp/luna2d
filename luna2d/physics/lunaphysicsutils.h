//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#pragma once

#include "lunalua.h"
#include <Box2D.h>

namespace luna2d{

class LUNAPhysicsUtils
{
private:
	LUNAPhysicsUtils() = delete;

private:
	static float pixelsInMeter;

public:
	static void SetPixelsInMeter(float value);
	static float PixelsToMeters(float pixels);
	static float MetersToPixels(float meters);
	static b2Vec2 TableToB2Vector(const LuaTable& table); // Convert LuaTable to b2Vec2 with converting pixels to meters
	static LuaTable B2VectorToTable(const b2Vec2& vec); // Convert b2Vec2 to LuaTable with converting meters to pixels
};

}
