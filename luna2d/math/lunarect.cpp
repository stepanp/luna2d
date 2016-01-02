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

#include "lunarect.h"

using namespace luna2d;

LUNARect::LUNARect() : LUNARect(0, 0, 0, 0)
{
}

LUNARect::LUNARect(float x, float y, float width, float height) :
	x(x), y(y), width(width), height(height)
{
}

// Conctuctor for creating instance in lua
int LUNARect::LuaConstruct(lua_State* luaVm)
{
	int argsCount = lua_gettop(luaVm);

	// Make empty rect
	if(argsCount == 0) LuaStack<LUNARect>::Push(luaVm, LUNARect());

	// Make copy of given rect
	else if(argsCount == 1) LuaStack<LUNARect>::Push(luaVm, LuaStack<LUNARect>::Pop(luaVm, 1));

	// Make rect from given params
	else
	{
		float x = LuaStack<float>::Pop(luaVm, 1);
		float y = LuaStack<float>::Pop(luaVm, 2);
		float width = LuaStack<float>::Pop(luaVm, 3);
		float height = LuaStack<float>::Pop(luaVm, 4);
		LuaStack<LUNARect>::Push(luaVm, LUNARect(x, y, width, height));
	}

	return 1;
}
