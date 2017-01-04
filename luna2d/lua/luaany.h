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

#include "luaobject.h"

namespace luna2d{

class LuaTable;
class LuaFunction;

class LuaAny : public LuaObject
{
public:
	LuaAny();
	LuaAny(const LuaNil& value);
	LuaAny(LuaScript* lua);
	LuaAny(lua_State* luaVm);
	LuaAny(lua_State* luaVm, int ref);
	LuaAny(const LuaAny& type);

	template<typename T>
	LuaAny(LuaScript* lua, const T& t) : LuaAny(lua)
	{
		FromValue(t);
	}

public:
	int GetType() const;

	template<typename T>
	T To() const
	{
		lua_State* luaVm = ref->GetLuaVm();

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef()); // Push variable to stack
		T&& ret = LuaStack<T>::Pop(luaVm, -1);
		lua_pop(luaVm, 1); // Remove variable from stack

		return ret;
	}

	template<typename T>
	void FromValue(const T& t)
	{
		lua_State* luaVm = ref->GetLuaVm();

		LuaStack<T>::Push(luaVm, t);
		ref = std::make_shared<LuaRef>(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	}

	LuaAny& operator=(const LuaAny& type);

	// Helpers for most frequently used types
	inline bool ToBool() const { return To<bool>(); }
	inline int ToInt() const { return To<int>(); }
	inline float ToFloat() const { return To<float>(); }
	inline std::string ToString() const { return To<std::string>(); }

	// Following functions declared in .cpp to avoid circular dependencies
	LuaTable ToTable() const;
	LuaFunction ToFunction() const;
};


template<>
struct LuaStack<LuaAny>
{
	static void Push(lua_State* luaVm, LuaAny type)
	{
		LuaStack<LuaRef*>::Push(luaVm, type.GetRef().get());
	}

	static LuaAny Pop(lua_State* luaVm, int index = -1)
	{
		// Push variable from index to top of stack
		// because "luaL_ref" can get ref only from top of stack
		lua_pushvalue(luaVm, index);

		int ref = luaL_ref(luaVm, LUA_REGISTRYINDEX);
		return LuaAny(luaVm, ref);
	}
};

}
