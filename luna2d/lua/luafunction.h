//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

#include "luaproxy.h"
#include "luaobject.h"

namespace luna2d{

class LuaFunction : public LuaObject
{
public:
	LuaFunction(const LuaNil& value);
	LuaFunction(LuaScript* lua);
	LuaFunction(lua_State* luaVm);
	LuaFunction(lua_State* luaVm, int ref);
	LuaFunction(const LuaFunction& fn);
	LuaFunction(LuaFunction&& fn);

	// Binding constructors
	template<typename Ret, typename ... Args>
	LuaFunction(LuaScript *lua, Ret (*func)(Args ...)) : LuaFunction(lua)
	{
		Bind<Ret, Args...>(func);
	}

	template<typename Ret, typename Class, typename ... Args>
	LuaFunction(LuaScript* lua, Class* obj, Ret (Class::*method)(Args ...))  : LuaFunction(lua)
	{
		Bind<Ret, Class, Args...>(obj, method);
	}

private:
	void PushArgs(lua_State*) const {}

	template<typename T>
	void PushArgs(lua_State* luaVm, const T& t) const
	{
		LuaStack<T>::Push(luaVm, t);
	}

	template<typename T, typename ... Args>
	void PushArgs(lua_State* luaVm, const T& t, const Args& ... args) const
	{
		LuaStack<T>::Push(luaVm, t);
		PushArgs(luaVm, args...);
	}

	// Push proxy object to lua stack and set GC handler
	// for proxy object can be deleted if binded function was collected by GC
	template<typename Proxy>
	void PushProxy(lua_State *luaVm, Proxy *proxy) const
	{
		// Wrap proxy object to lua userdata
		Proxy **userData = static_cast<Proxy**>(lua_newuserdata(luaVm, sizeof(Proxy*)));
		*userData = proxy;

		// Set metatable to userdata with GC handler
		luaL_newmetatable(luaVm, "func_proxy_mt"); // Use one metatable for all proxy objects
		lua_pushcclosure(luaVm, &LuaProxy::OnGc, 0);
		lua_setfield(luaVm, -2, "__gc");
		lua_setmetatable(luaVm, -2);
	}

public:
	// Bind function to lua
	template<typename Ret, typename ... Args>
	void Bind(Ret (*func)(Args ...))
	{
		lua_State *luaVm = ref->GetLuaVm();

		PushProxy(luaVm, new LuaFunctionProxy<Ret, Args...>(func));
		lua_pushcclosure(luaVm, LuaFunctionProxy<Ret, Args...>::Callback, 1);

		ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	}

	// Bind method to lua
	template<typename Ret, typename Class, typename ... Args>
	void Bind(Class* obj, Ret (Class::*method)(Args ...))
	{
		lua_State *luaVm = ref->GetLuaVm();

		PushProxy(luaVm, new LuaMethodProxy<Ret, Class, Args...>(obj, method));
		lua_pushcclosure(luaVm, LuaMethodProxy<Ret, Class, Args...>::Callback, 1);

		ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	}

	// Bind method to special proxy
	// Pointer to object not store with pointer to method,
	// but passing as fist param of lua function
	// Uses for binding methods in "luaclass.h"
	template<typename Ret, typename Class, typename ... Args>
	void Bind(Ret (Class::*method)(Args ...))
	{
		lua_State *luaVm = ref->GetLuaVm();

		PushProxy(luaVm, new LuaClassProxy<Ret, Class, Args...>(method));
		lua_pushcclosure(luaVm, LuaClassProxy<Ret, Class, Args...>::Callback, 1);

		ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	}

	// Call function and get return value
	template<typename Ret, typename ... Args>
	Ret Call(Args ... args) const
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_pushcfunction(luaVm, &LuaScript::ErrorHandler); // Set error handler
		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		PushArgs(luaVm, args...);
		lua_pcall(luaVm, sizeof ... (Args), 1, -(sizeof ... (Args) + 2));

		Ret&& ret = LuaStack<Ret>::Pop(luaVm);
		lua_pop(luaVm, 2); // Pop error handler and return value from stack

		return ret;
	}

	// Call function without return value
	template<typename ... Args>
	void CallVoid(Args ... args) const
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_pushcfunction(luaVm, &LuaScript::ErrorHandler); // Set error handler
		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		PushArgs(luaVm, args...);
		lua_pcall(luaVm, sizeof ... (Args), 0, -(sizeof ... (Args) + 2));
		lua_pop(luaVm, 1); // Pop error handler from stack
	}

	LuaFunction& operator=(const LuaFunction& fn);
};


template<>
struct LuaStack<LuaFunction>
{
	static void Push(lua_State* luaVm, const LuaFunction& fn)
	{
		LuaStack<LuaRef*>::Push(luaVm, fn.GetRef().get());
	}

	static LuaFunction Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isfunction(luaVm, index)) return std::move(LuaFunction(luaVm)); // Return non-binded lua function

		// Push function from index to top of stack
		// because "luaL_ref" can get ref only from top of stack
		lua_pushvalue(luaVm, index);

		int ref = luaL_ref(luaVm, LUA_REGISTRYINDEX);
		return std::move(LuaFunction(luaVm, ref));
	}
};

}
