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

namespace luna2d{

class LUNARect
{
public:
	LUNARect();
	LUNARect(float x, float y, float width, float height);
	static int LuaConstruct(lua_State* luaVm); // Conctuctor for creating instance in lua

public:
	float x, y, width, height;
};

template<>
struct LuaStack<LUNARect>
{
	static void Push(lua_State* luaVm, const LUNARect& rect)
	{
		lua_createtable(luaVm, 0, 4);
		lua_pushliteral(luaVm, "x");
		lua_pushnumber(luaVm, rect.x);
		lua_rawset(luaVm, -3);
		lua_pushliteral(luaVm, "y");
		lua_pushnumber(luaVm, rect.y);
		lua_rawset(luaVm, -3);
		lua_pushliteral(luaVm, "width");
		lua_pushnumber(luaVm, rect.width);
		lua_rawset(luaVm, -3);
		lua_pushliteral(luaVm, "height");
		lua_pushnumber(luaVm, rect.height);
		lua_rawset(luaVm, -3);
	}

	static LUNARect Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_istable(luaVm, index)) return LUNARect();

		lua_pushvalue(luaVm, index);
		lua_pushliteral(luaVm, "x");
		lua_rawget(luaVm, -2);
		lua_pushliteral(luaVm, "y");
		lua_rawget(luaVm, -3);
		lua_pushliteral(luaVm, "width");
		lua_rawget(luaVm, -4);
		lua_pushliteral(luaVm, "height");
		lua_rawget(luaVm, -5);

		LUNARect ret(lua_tonumber(luaVm, -4), lua_tonumber(luaVm, -3),
			lua_tonumber(luaVm, -2), lua_tonumber(luaVm, -1));
		lua_pop(luaVm, 5);

		return ret;
	}
};

}
