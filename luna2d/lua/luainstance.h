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

#include "luatable.h"
#include "luaptr.h"

namespace luna2d{

//---------------------------------------------------------------------------
// Unified interface for working with table-based and user-data based objects
//---------------------------------------------------------------------------
class LuaInstance : public LuaObject
{
public:
	LuaInstance();
	LuaInstance(const LuaNil& value);
	LuaInstance(LuaScript* lua);
	LuaInstance(lua_State* luaVm);
	LuaInstance(lua_State* luaVm, int ref, bool isUserdata);
	LuaInstance(const LuaInstance& instance);

private:
	bool isUserdata = false;

public:
	bool IsUserdata() const;
	bool HasField(const std::string& name) const;
	LuaFunction GetFunction(const std::string& name) const;
	LuaTable ToTable() const;

	template<typename T>
	std::shared_ptr<T> ToUserdata() const
	{
		lua_State* luaVm = ref->GetLuaVm();

		LuaStack<LuaRef*>::Push(luaVm, ref.get());
		std::shared_ptr<T> ret = LuaStack<std::shared_ptr<T>>::Pop(luaVm, -1);
		lua_pop(luaVm, 1);

		return ret;
	}

	template<typename Ret, typename ... Args>
	Ret CallMethod(const std::string& name, const Args& ... args) const
	{
		LuaFunction fn = GetFunction(name);
		if(fn) return fn.Call<Ret>(ref.get(), args...);
		else return Ret();
	}

	template<typename ... Args>
	void CallMethodVoid(const std::string& name, const Args& ... args) const
	{
		LuaFunction fn = GetFunction(name);
		if(fn) fn.CallVoid(ref.get(), args...);
	}
};


template<>
struct LuaStack<LuaInstance>
{
	static void Push(lua_State* luaVm, const LuaInstance& instance)
	{
		LuaStack<LuaRef*>::Push(luaVm, instance.GetRef().get());
	}

	static LuaInstance Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_istable(luaVm, index) && !lua_isuserdata(luaVm, index)) return nil;

		// Push table from index to top of stack
		// because "luaL_ref" can get ref only from top of stack
		lua_pushvalue(luaVm, index);

		int ref = luaL_ref(luaVm, LUA_REGISTRYINDEX);
		return LuaInstance(luaVm, ref, !!lua_isuserdata(luaVm, -1));
	}
};

}
