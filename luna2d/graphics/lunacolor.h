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

#include "lunalua.h"

namespace luna2d{

class LUNAColor
{
private:
	LUNAColor(float r, float g, float b, float a = 1.0f);

public:
	LUNAColor();

public:
	float r, g, b, a;


public:
	// Get color valus as bytes
	unsigned char GetR() const;
	unsigned char GetG() const;
	unsigned char GetB() const;
	unsigned char GetA() const;

	// Get color as uint32_t
	uint32_t GetUint32() const;

	static float ByteToFloat(unsigned char value); // Convert value from byte format(0-255) to float format(0.0f-1.0f)

	static LUNAColor Rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255); // Values from 0 to 255
	static LUNAColor RgbFloat(float r, float g, float b, float a = 1.0f); // Values from 0.0f to 1.0f
	static LUNAColor Hex(int hex, float a = 1.0f); // From hex format(0xRRGGBB), alpha is optional
	static LUNAColor Uint32(uint32_t color); // From uint32_t

	// Frequently used colors
	static LUNAColor WHITE, BLACK, GRAY, RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, TRANSPARENT;
};


template<>
struct LuaStack<LUNAColor>
{
	static void Push(lua_State* luaVm, const LUNAColor& color)
	{
		LuaTable tblColor(luaVm);
		tblColor.SetArrayField<int>(1, color.GetR());
		tblColor.SetArrayField<int>(2, color.GetG());
		tblColor.SetArrayField<int>(3, color.GetB());
		LuaStack<LuaTable>::Push(luaVm, tblColor);
	}

	static LUNAColor Pop(lua_State* luaVm, int index = -1)
	{
		LuaTable tblColor = LuaStack<LuaTable>::Pop(luaVm, index);
		if(tblColor == nil) return LUNAColor();

		unsigned char r = (unsigned char)tblColor.GetArrayField<int>(1);
		unsigned char g = (unsigned char)tblColor.GetArrayField<int>(2);
		unsigned char b = (unsigned char)tblColor.GetArrayField<int>(3);
		return LUNAColor::Rgb(r, g, b);
	}
};

}
