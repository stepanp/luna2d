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

#include "luatable.h"
#include "luareflection.h"

namespace luna2d{

//--------------------------------------
// Helper for binding C++ classes to lua
//--------------------------------------
template<typename Class>
class LuaClass : public LuaTable
{
#if LUNA_PLATFORM != LUNA_PLATFORM_WP
	static_assert(Class::_AssertIsBaseOf(), "");
#endif

public:
	LuaClass(LuaScript *lua) : LuaTable(lua->GetLuaVm(), LUA_NOREF)
	{
		lua_State *luaVm = lua->GetLuaVm();

		// Create metatable for Class in lua registry
		// If metatable for Class already exists, just get it from registry
		bool alreadyRegistred = luaL_newmetatable(luaVm, Class::_GetTypeName()) == 0;

		// Now, this object of LuaClass is represents the metatable
		ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
		if(alreadyRegistred) return;

		// Set "__index" and "__gc" methamethods
		SetField("__index", &OnIndex);
		SetField("__gc", &OnGc);

		// Save userdata type to metatable
		// For safe-casting from lua userdata to C++ object
		// SEE: "luaptr.h"
		SetField<LuaLightUserdata>("_typeId", reinterpret_cast<LuaLightUserdata>(LuaGetTypeId<Class>()));

		// Save metatable of base class in this table
		// For support inheritance in lua and support polyphormism for casting from lua to C++ object
		const char* baseTypeName = Class::_GetBaseTypeName();
		if(baseTypeName)
		{
			luaL_getmetatable(luaVm, baseTypeName);

			if(lua_isnil(luaVm, -1))
			{
				LUNA_LOGE("Metatable for base class \"%s\" must be initialized before metatable for derived class \"%s\"",
					baseTypeName, Class::_GetTypeName());
				lua_pop(luaVm, 1); // Remove nil from stack
			}
			else
			{
				SetField("_baseClass", LuaStack<LuaTable>::Pop(luaVm, -1));
				lua_pop(luaVm, 1); // Remove metatable from stack
			}
		}

		// Deny change class table from lua
		MakeReadOnly();
	}

private:
	// Wrapper for constructor
	// This method is invoked when lua constructor("__call" metamethod") is called
	template<typename ... Args>
	static std::shared_ptr<Class> Construct(LuaNil, Args ... args)
	{
		return std::make_shared<Class>(args ...);
	}

	// Handler for "__index" metamethod
	// Lua stack:
	// -2 userdata
	// -1 key
	static int OnIndex(lua_State* luaVm)
	{
		// Get value from metatable
		lua_getmetatable(luaVm, -2);
		lua_pushvalue(luaVm, -2); // Push copy of key to front of metatable
		lua_rawget(luaVm, -2);

		// If value found, return it
		if(!lua_isnil(luaVm, -1))
		{
			lua_remove(luaVm, -2); // Remove metatable from stack
			return 1;
		}

		lua_pop(luaVm, 1); // Remove nil from stack

		// If current metatable hasn't value at given key,
		// try get value from base classes
		while(true)
		{
			// Get baseclass metatable
			lua_pushliteral(luaVm, "_baseClass");
			lua_rawget(luaVm, -2);
			lua_remove(luaVm, -2); // Remove previous metatable from stack

			// This userdata hasn't baseclass
			if(lua_isnil(luaVm, -1)) return 1;

			lua_pushvalue(luaVm, -2); // Push copy of key to front of metatable
			lua_rawget(luaVm, -2);

			// If value found, return it
			if(!lua_isnil(luaVm, -1))
			{
				lua_remove(luaVm, -2); // Remove metatable from stack
				return 1;
			}
			else lua_pop(luaVm, 1); // Remove nil from stack
		}

		return 1;
	}

	// Handler for "__gc" metamethod
	// Calls when userdata object was collected by lua GC
	static int OnGc(lua_State* luaVm)
	{
		if(!lua_isuserdata(luaVm, 1)) return 0;

		std::shared_ptr<Class>* ptr = *static_cast<std::shared_ptr<Class>**>(lua_touserdata(luaVm, 1));

		// Release ref to lua userdata in C++ object
		if(ptr->use_count() > 0)
		{
			auto ref = ptr->get()->_GetLuaRef();
			if(ref) ref->Release();
		}

		delete ptr;

		return 0;
	}

public:
	template<typename ... Args>
	void SetConstructor()
	{
		lua_State *luaVm = ref->GetLuaVm();

		LuaFunction fnConstruct(luaVm);
		fnConstruct.Bind<std::shared_ptr<Class>, LuaNil, Args...>(&LuaClass<Class>::Construct);

		LuaTable meta(luaVm);
		meta.SetField("__call", fnConstruct);
		SetMetatable(meta);
	}

	template<typename Ret, typename ... Args>
	void SetMethod(const std::string& name, Ret (Class::*method)(Args ...))
	{
		LuaFunction fn(ref->GetLuaVm());
		fn.Bind<Ret, Class, Args...>(method);
		SetField(name, fn, true);
	}

	template<typename Ret, typename ... Args>
	void SetMethod(const std::string& name, Ret (Class::*method)(Args ...) const)
	{
		LuaFunction fn(ref->GetLuaVm());
		fn.Bind<Ret, Class, Args...>(method);
		SetField(name, fn, true);
	}
};

// Use same implementation of LuaStack as for LuaTable
template<typename Class>
struct LuaStack<LuaClass<Class>> : public LuaStack<LuaTable> {};

}
