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

#include "luatable.h"
#include <type_traits>

// VS2013 not supports constexpr 
#if LUNA_PLATFORM == LUNA_PLATFORM_WP
	#define constexpr	
#endif

#define LUNA_USERDATA(cls) \
	static_assert(std::is_class<cls>::value, "Type \"" #cls "\" is must be a class"); \
	protected: \
		std::shared_ptr<LuaWeakRef> _ref = std::make_shared<LuaWeakRef>(nil); \
	public: \
		void _SetLuaRef(const std::shared_ptr<LuaWeakRef>& ref) { _ref = ref; } \
		std::shared_ptr<LuaWeakRef> _GetLuaRef() { return _ref; } \
		/* Forced release shared_ptr passed to Lua */ \
		/* i.e. userdata object in lua will still exists, but references nothing */ \
		/* and it will can be safety deleted by GC */ \
		void _KillLuaRef() \
		{ \
			if(!_ref || *_ref == nil) return; \
			\
			lua_State* luaVm = _ref->GetLuaVm(); \
			LuaStack<LuaWeakRef*>::Push(luaVm, _ref.get()); \
			std::shared_ptr<cls>* ptr = *static_cast<std::shared_ptr<cls>**>(lua_touserdata(luaVm, -1)); \
			lua_pop(luaVm, 1); \
			\
			ptr->reset(); \
			_ref->Release(); \
		} \
		static const char* _GetUserdataType() { return #cls; } \
		static const char* _GetBaseClassType() { return nullptr; } \
		constexpr static bool _CheckIsBaseOf() { return true; } \
	private: \

#define LUNA_USERDATA_DERIVED(basecls, cls) \
	static_assert(std::is_class<basecls>::value && std::is_class<cls>::value, \
		"Types \"" #basecls "\" and \"" #cls "\" are must be a classes"); \
	public:  \
		static const char* _GetUserdataType() { return #cls; } \
		static const char* _GetBaseClassType() { return #basecls; } \
		constexpr static bool _CheckIsBaseOf() \
		{ \
			static_assert(std::is_base_of<basecls,cls>::value, "\"" #basecls "\" is not base class of \"" #cls "\""); \
			return true; \
		} \
	private:


namespace luna2d{

//--------------------------------------
// Helper for binding C++ classes to lua
//--------------------------------------
template<typename Class>
class LuaClass : public LuaTable
{
#if LUNA_PLATFORM != LUNA_PLATFORM_WP
	static_assert(Class::_CheckIsBaseOf(), "");
#endif

	typedef LuaDynamicType (Class::*IndexHandler)(const LuaDynamicType&);

public:
	LuaClass(LuaScript *lua) : LuaTable(lua->GetLuaVm(), LUA_NOREF)
	{
		lua_State *luaVm = lua->GetLuaVm();

		// Create metatable for Class in lua registry
		// If metatable for Class already exists, just get it from registry
		bool alreadyRegistred = luaL_newmetatable(luaVm, Class::_GetUserdataType()) == 0;

		// Now, this object of LuaClass is represents the metatable
		ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
		if(alreadyRegistred) return;

		// Set "__index" and "__gc" methamethods
		SetField("__index", &OnIndex);
		SetField("__newindex", &OnNewIndex);
		SetField("__gc", &OnGc);

		// Add userdata type to metatable
		// Userdata type stored in metatable as pointer to string
		// for fast checking type at the time of casting from lua to C++ object
		// SEE: LuaStack<T*>::Push/Pop
		const char* userdataType = Class::_GetUserdataType();
		SetField("_userdataType", static_cast<LuaLightUserdata>(const_cast<char*>(userdataType)));

		// Save metatable of base class in this table
		// For support inheritance in lua and support polyphormism for casting from lua to C++ object
		const char* baseClassType = Class::_GetBaseClassType();
		if(baseClassType)
		{
			luaL_getmetatable(luaVm, baseClassType);

			if(lua_isnil(luaVm, -1))
			{
				LUNA_LOGE("Metatable for base class \"%s\" must be initialized before metatable for derived class \"%s\"", userdataType, baseClassType);
				lua_pop(luaVm, 1); // Remove nil from stack
			}
			else
			{
				LuaTable baseMeta(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
				SetField("_baseClass", baseMeta);
			}
		}

		// Deny change class table from lua
		MakeReadOnly();
	}

private:
	static IndexHandler indexHandler;

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
		// Try call index handler for given userdata object
		if(indexHandler)
		{
			Class *obj = LuaStack<std::shared_ptr<Class>>::Pop(luaVm, -2).get();
			if(obj)
			{
				const LuaDynamicType& key = LuaStack<LuaDynamicType>::Pop(luaVm, -1);
				const LuaDynamicType& ret = (obj->*LuaClass<Class>::indexHandler)(key);

				if(ret != nil)
				{
					LuaStack<LuaDynamicType>::Push(luaVm, ret);
					return 1;
				}
			}
		}

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

	static int OnNewIndex(lua_State* luaVm)
	{
		return 0;
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

	void SetIndexHandler(IndexHandler indexHandler)
	{
		LuaClass<Class>::indexHandler = indexHandler;
	}
};

// Initialize static members of LuaClass
template<typename Class>
LuaDynamicType (Class::*LuaClass<Class>::indexHandler)(const LuaDynamicType&) = nullptr;

// Use same implementation of LuaStack as for LuaTable
template<typename Class>
struct LuaStack<LuaClass<Class>> : public LuaStack<LuaTable> {};

}
