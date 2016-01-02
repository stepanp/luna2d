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

#include "lunalua.h"
#include "lunamath.h"

namespace luna2d{

template<typename T>
class LUNARange
{
public:
	LUNARange() : LUNARange(T()) {}
	LUNARange(T value) : LUNARange(value, value) {}
	LUNARange(T min, T max) : min(min), max(max) {}

public:
	T min, max;
};

typedef LUNARange<float> LUNARangeFloat;
typedef LUNARange<int> LUNARangeInt;


template<typename T>
struct LuaStack<LUNARange<T>>
{
	inline static void Push(lua_State* luaVm, const LUNARange<T>& range)
	{
		LuaTable tblRange(luaVm);

		tblRange.SetArrayField(1, range.min);
		tblRange.SetArrayField(2, range.max);

		LuaStack<LuaTable>::Push(luaVm, tblRange);
	}

	inline static LUNARange<T> Pop(lua_State* luaVm, int index = -1)
	{
		if(lua_istable(luaVm, index))
		{
			LuaTable tblRange = LuaStack<LuaTable>::Pop(luaVm, index);
			return LUNARange<T>(tblRange.GetArrayField<T>(1), tblRange.GetArrayField<T>(2));
		}

		else if(lua_isnumber(luaVm, index))
		{
			return LUNARange<T>(LuaStack<T>::Pop(luaVm, index));
		}

		return LUNARange<T>();
	}
};

}
