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

#include "luastack.h"
#include "utils/lunaindexlist.h"
#include <memory>

namespace luna2d{

// Helper for getting raw pointer from userdata
template<typename Class>
Class* SafeGetRawPointer(lua_State* luaVm)
{
	if(!lua_isuserdata(luaVm, 1))
	{
		LUNA_LOGE("First argument is not userdata. Possibly method called through \".\" instead of \":\" operator?");
		return 0;
	}

	// Check for it valid engine userdata
	lua_getuservalue(luaVm, 1);
	if(lua_isnil(luaVm, -1))
	{
		LUNA_LOGE("First argument is not valid engine userdata");
		lua_pop(luaVm, 1);
		return nullptr;
	}

	// Check for weak pointer
	int LUA_USERVALUE_WEAK = 1; // TODO: Make LuaUserdata::* functions useful in "luaproxy.h"
	lua_rawgeti(luaVm, -1, LUA_USERVALUE_WEAK);
	bool isWeak = lua_toboolean(luaVm, -1);
	lua_pop(luaVm, 2);

	if(isWeak)
	{
		std::weak_ptr<Class>* weakObj = *static_cast<std::weak_ptr<Class>**>(lua_touserdata(luaVm, 1));
		if(weakObj->expired())
		{
			LUNA_LOGE("Attempt to call method of exprited weak_ptr userdata");
			return nullptr;
		}
		return weakObj->lock().get();
	}
	else
	{
		return *static_cast<Class**>(lua_touserdata(luaVm, 1));
	}
}

class LuaProxy
{
public:
	virtual ~LuaProxy() {}

public:
	// GC handler for LuaProxy objects
	// Calls when binded function was collected by lua GC
	static int OnGc(lua_State *luaVm)
	{
		if(!lua_isuserdata(luaVm, 1)) return 0;

		// Proxy object is no longer needed, delete it
		LuaProxy *proxy = *static_cast<LuaProxy**>(lua_touserdata(luaVm, 1));
		delete proxy;

		return 0;
	}
};

template<typename Ret, typename ... Args>
class LuaFunctionProxy : public LuaProxy
{
	typedef LuaFunctionProxy<Ret, Args...> Proxy;
	typedef Ret (*Func)(Args ...);

public:
	LuaFunctionProxy(Func func)
	{
		this->func = func;
	}

private:
	Func func;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Call function
		LuaStack<Ret>::Push(luaVm, Call(proxy, luaVm, LUNAMakeIndexList<Args...>()));

		return 1; // Count of return values
	}

	template<size_t ... Index>
	static Ret Call(Proxy *proxy, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		return proxy->func(LuaStack<Args>::Pop(luaVm, Index + 1) ...);
	}
};


//--------------------------------------------------
// Specialization for functions without return value
//--------------------------------------------------
template<typename ... Args>
class LuaFunctionProxy<void, Args...> : public LuaProxy
{
	typedef LuaFunctionProxy<void, Args...> Proxy;
	typedef void (*Func)(Args ...);

public:
	LuaFunctionProxy(Func func)
	{
		this->func = func;
	}

private:
	Func func;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Call function without return value
		Call(proxy, luaVm, LUNAMakeIndexList<Args...>());

		return 0; // No return value
	}

	template<size_t ... Index>
	static void Call(Proxy *proxy, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		proxy->func(LuaStack<Args>::Pop(luaVm, Index + 1) ...);
	}
};


template<typename Ret, typename Class, typename ... Args>
class LuaMethodProxy : public LuaProxy
{
	typedef LuaMethodProxy<Ret, Class, Args...> Proxy;
	typedef Ret (Class::*Method)(Args ...);

public:
	LuaMethodProxy(Class *obj, Method method)
	{
		this->obj = obj;
		this->method = method;
	}

private:
	Class* obj;
	Method method;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Call method
		LuaStack<Ret>::Push(luaVm, Call(proxy, luaVm, LUNAMakeIndexList<Args...>()));

		return 1; // Count of return values
	}

	template<size_t ... Index>
	static Ret Call(Proxy *proxy, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		return (proxy->obj->*proxy->method)(LuaStack<Args>::Pop(luaVm, Index + 1) ...);
	}
};


//------------------------------------------------
// Specialization for methods without return value
//------------------------------------------------
template<typename Class, typename ... Args>
class LuaMethodProxy<void, Class, Args ...> : public LuaProxy
{
	typedef LuaMethodProxy<void, Class, Args...> Proxy;
	typedef void (Class::*Method)(Args ...);

public:
	LuaMethodProxy(Class *obj, Method method)
	{
		this->obj = obj;
		this->method = method;
	}

private:
	Class* obj;
	Method method;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Call method
		Call(proxy, luaVm, LUNAMakeIndexList<Args...>());

		return 0; // No return value
	}

	template<size_t ... Index>
	static void Call(Proxy *proxy, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		(proxy->obj->*proxy->method)(LuaStack<Args>::Pop(luaVm, Index + 1) ...);
	}
};


template<typename Ret, typename Class, typename ... Args>
class LuaClassProxy : public LuaProxy
{
	typedef LuaClassProxy<Ret, Class, Args...> Proxy;
	typedef Ret (Class::*Method)(Args ...);

public:
	LuaClassProxy(Method method)
	{
		this->method = method;
	}

private:
	Method method;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Get pointer to object from first method param
		Class *obj = SafeGetRawPointer<Class>(luaVm);
		if(!obj) return 0;

		// Call method
		LuaStack<Ret>::Push(luaVm, Call(obj, proxy->method, luaVm, LUNAMakeIndexList<Args...>()));

		return 1; // Count of return values
	}

	template<size_t ... Index>
	static Ret Call(Class *obj, Method method, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		return (obj->*method)(LuaStack<Args>::Pop(luaVm, Index + 2) ...);
	}
};


template<typename Class, typename ... Args>
class LuaClassProxy<void, Class, Args ...> : public LuaProxy
{
	typedef LuaClassProxy<void, Class, Args...> Proxy;
	typedef void (Class::*Method)(Args ...);

public:
	LuaClassProxy(Method method)
	{
		this->method = method;
	}

private:
	Method method;

public:
	static int Callback(lua_State *luaVm)
	{
		// Pop pointer to proxy object from stack
		if(!lua_isuserdata(luaVm, lua_upvalueindex(1))) return 0;
		Proxy *proxy = *static_cast<Proxy**>(lua_touserdata(luaVm, lua_upvalueindex(1)));

		// Get pointer to object from first method param
		Class *obj = SafeGetRawPointer<Class>(luaVm);
		if(!obj) return 0;

		// Call method
		Call(obj, proxy->method, luaVm, LUNAMakeIndexList<Args...>());

		return 0; // No return value
	}

	template<size_t ... Index>
	static void Call(Class *obj, Method method, lua_State *luaVm, LUNAIndexList<Index ...>)
	{
		(obj->*method)(LuaStack<Args>::Pop(luaVm, Index + 2) ...);
	}
};


}
