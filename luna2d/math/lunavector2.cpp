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

#include "lunavector2.h"

using namespace luna2d;

int LuaVec2::Construct(lua_State *luaVm)
{
	int argsCount = lua_gettop(luaVm);

	// Make empty vector
	if(argsCount == 1) LuaStack<glm::vec2>::Push(luaVm, glm::vec2());

	// Make copy of given vector
	else if(argsCount == 2) LuaStack<glm::vec2>::Push(luaVm, LuaStack<glm::vec2>::Pop(luaVm, 2));

	// Make vector from given coordinates
	else
	{
		float x = LuaStack<float>::Pop(luaVm, 2);
		float y = LuaStack<float>::Pop(luaVm, 3);
		LuaStack<glm::vec2>::Push(luaVm, glm::vec2(x, y));
	}

	return 1;
}

// Add given vector to this vector
int LuaVec2::Add(lua_State *luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<glm::vec2>::Set(luaVm, vec1 += vec2, 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Substract given vector from this vector
int LuaVec2::Sub(lua_State *luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<glm::vec2>::Set(luaVm, vec1 -= vec2, 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Multiple this vector to scalar
int LuaVec2::Scale(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	float scale = LuaStack<float>::Pop(luaVm, 2);

	LuaStack<glm::vec2>::Set(luaVm, vec *= scale, 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Rotate this vector to given angle (in degrees)
int LuaVec2::Rotate(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	float angle = LuaStack<float>::Pop(luaVm, 2);

	LuaStack<glm::vec2>::Set(luaVm, glm::rotate(vec, glm::radians(angle)), 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Rotate this vector to given angle (in radians)
int LuaVec2::Rotater(lua_State* luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	float angle = LuaStack<float>::Pop(luaVm, 2);

	LuaStack<glm::vec2>::Set(luaVm, glm::rotate(vec, angle), 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Normalize this vector
int LuaVec2::Nor(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	LuaStack<glm::vec2>::Set(luaVm, glm::normalize(vec), 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Make perpendicular of this vector
int LuaVec2::Perp(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	LuaStack<glm::vec2>::Set(luaVm, glm::vec2(-vec.y, vec.x), 1);
	lua_pushvalue(luaVm, 1);
	return 1;
}

// Get angle of this vector (in degrees)
int LuaVec2::Angle(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);

	if(glm::all(glm::lessThan(glm::abs(vec), glm::vec2(GLM_EPSILON))))
	{
		LuaStack<float>::Push(luaVm, 0.0f);
	}
	else
	{
		float angle = glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vec));
		LuaStack<float>::Push(luaVm, glm::degrees(angle));
	}
	return 1;
}

// Get angle of this vector (in radians)
int LuaVec2::Angler(lua_State* luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);

	if(glm::all(glm::lessThan(glm::abs(vec), glm::vec2(GLM_EPSILON))))
	{
		LuaStack<float>::Push(luaVm, 0.0f);
	}
	else
	{
		float angle = glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vec));
		LuaStack<float>::Push(luaVm, angle);
	}
	return 1;
}

// Get lenght of this vector
int LuaVec2::Len(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	LuaStack<float>::Push(luaVm, glm::length(vec));
	return 1;
}

// Get squared lenght of this vector
int LuaVec2::LenSqr(lua_State *luaVm)
{
	glm::vec2 vec = LuaStack<glm::vec2>::Pop(luaVm, 1);
	LuaStack<float>::Push(luaVm, glm::length2(vec));
	return 1;
}

// Get distance between this and given vectors
int LuaVec2::Dist(lua_State* luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<float>::Push(luaVm, glm::distance(vec1, vec2));
	return 1;
}

// Get squared distance between this and given vectors
int LuaVec2::DistSqr(lua_State* luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<float>::Push(luaVm, glm::distance2(vec1, vec2));
	return 1;
}

// Get cross product between this and given vectors
int LuaVec2::Cross(lua_State *luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<float>::Push(luaVm, vec1.x * vec2.y - vec2.x * vec1.y);
	return 1;
}

// Get dot product between this and given vectors
int LuaVec2::Dot(lua_State *luaVm)
{
	glm::vec2 vec1 = LuaStack<glm::vec2>::Pop(luaVm, 1);
	glm::vec2 vec2 = LuaStack<glm::vec2>::Pop(luaVm, 2);

	LuaStack<float>::Push(luaVm, glm::dot(vec1, vec2));
	return 1;
}

// Make copy of this vector
int LuaVec2::Copy(lua_State *luaVm)
{
	LuaStack<glm::vec2>::Push(luaVm, LuaStack<glm::vec2>::Pop(luaVm, 1));
	return 1;
}
