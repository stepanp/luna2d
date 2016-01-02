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
#include "lunaglm.h"

namespace luna2d{

const std::string VECTOR2_METATABLE_NAME = "glm::vec2";

//--------------------------------------------------
// Binding glm::vec2 to lua
// For maximize perfomance LuaClass<T> is not used
//--------------------------------------------------
class LuaVector2
{
public:
	static int Construct(lua_State* luaVm); // Constructor
	static int Add(lua_State* luaVm); // Add given vector to this vector
	static int Sub(lua_State* luaVm);  // Substract given vector from this vector
	static int Scale(lua_State* luaVm); // Scale this vector to scalar
	static int Rotate(lua_State* luaVm); // Multiple this vector to given angle (in degrees)
	static int Rotater(lua_State* luaVm); // Rotate this vector to given angle (in radians)
	static int Nor(lua_State* luaVm); // Normalize this vector
	static int Perp(lua_State* luaVm); // Make perpendicular of this vector
	static int Angle(lua_State* luaVm); // Get angle of this vector (in degrees)
	static int Angler(lua_State* luaVm); // Get angle of this vector (in radians)
	static int Len(lua_State* luaVm); // Get lenght of this vector
	static int LenSqr(lua_State* luaVm); // Get squared lenght of this vector
	static int Dist(lua_State* luaVm); // Get distance between this and given vectors
	static int DistSqr(lua_State* luaVm); // Get squared distance between this and given vectors
	static int Cross(lua_State* luaVm); // Get cross prodict between this and given vectors
	static int Dot(lua_State* luaVm); // Get dot prodict between this and given vectors
	static int Copy(lua_State* luaVm); // Make copy of this vector
};

template<>
struct LuaStack<glm::vec2>
{
	static void Push(lua_State* luaVm, const glm::vec2& vec)
	{
		lua_createtable(luaVm, 0, 2);
		lua_pushliteral(luaVm, "x");
		lua_pushnumber(luaVm, vec.x);
		lua_rawset(luaVm, -3);
		lua_pushliteral(luaVm, "y");
		lua_pushnumber(luaVm, vec.y);
		lua_rawset(luaVm, -3);

		luaL_getmetatable(luaVm, VECTOR2_METATABLE_NAME.c_str());
		lua_setmetatable(luaVm, -2);
	}

	// Set new x and y values for table at index of lua stack
	// To avoid creation a copy of vector table
	static void Set(lua_State* luaVm, const glm::vec2& vec, int index)
	{
		lua_pushvalue(luaVm, index);
		lua_pushliteral(luaVm, "x");
		lua_pushnumber(luaVm, vec.x);
		lua_rawset(luaVm, -3);
		lua_pushliteral(luaVm, "y");
		lua_pushnumber(luaVm, vec.y);
		lua_rawset(luaVm, -3);
		lua_pop(luaVm, 1);
	}

	static glm::vec2 Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_istable(luaVm, index)) return glm::vec2();

		lua_pushvalue(luaVm, index);
		lua_pushliteral(luaVm, "y");
		lua_rawget(luaVm, -2);
		lua_pushliteral(luaVm, "x");
		lua_rawget(luaVm, -3);

		glm::vec2 ret(lua_tonumber(luaVm, -1), lua_tonumber(luaVm, -2));
		lua_pop(luaVm, 3);

		return ret;
	}
};

}
