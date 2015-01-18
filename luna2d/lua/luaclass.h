//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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
#define LUNA_CHECK_BASE_OF(cls)
#define LUNA_CHECK_BASE_OF_DERRIVED(basecls, cls)
#else
#define LUNA_CHECK_BASE_OF(cls) constexpr static bool _CheckIsBaseOf() { return true; }
#define LUNA_CHECK_BASE_OF_DERRIVED(basecls, cls) \
	constexpr static bool _CheckIsBaseOf() \
	{ \
		static_assert(std::is_base_of<basecls,cls>::value, "\"" #basecls "\" is not base class of \"" #cls "\""); \
		return true; \
	}
#endif

#define LUNA_USERDATA(cls) \
	static_assert(std::is_class<cls>::value, "Type \"" #cls "\" is must be a class"); \
	protected: \
		std::shared_ptr<LuaWeakRef> _ref = std::make_shared<LuaWeakRef>(nil); \
	public: \
		void _SetLuaRef(const std::shared_ptr<LuaWeakRef>& ref) { _ref = ref; } \
		std::shared_ptr<LuaWeakRef> _GetLuaRef() { return _ref; } \
		static const char* _GetUserdataType() { return #cls; } \
		static const char* _GetBaseClassType() { return nullptr; } \
		LUNA_CHECK_BASE_OF(cls) \
	private: \

#define LUNA_USERDATA_DERIVED(basecls, cls) \
	static_assert(std::is_class<basecls>::value && std::is_class<cls>::value, \
		"Types \"" #basecls "\" and \"" #cls "\" are must be a classes"); \
	public:  \
		static const char* _GetUserdataType() { return #cls; } \
		static const char* _GetBaseClassType() { return #basecls; } \
		LUNA_CHECK_BASE_OF_DERRIVED(basecls, cls) \
	private:


namespace luna2d{

const int LUA_USERVALUE_WEAK = 1;


//-------------------------------
// Namespace for helper functions
// for work with userdata
//-------------------------------
struct LuaUserdata
{
	// Check for given lua value is userdata with given type
	// "Type" - can be only class having LUNA_USERDATA/LUNA_USERDATA_DERIVED macro
	template<typename Type>
	static bool IsA(const LuaDynamicType& value)
	{
		if(value.GetType() != LUA_TUSERDATA) return false;

		const char *userdataType = GetType(value); // Userdata type from lua value
		const char *classType = Type::_GetUserdataType(); // Userdata type from given class type

		return userdataType == classType;
	}

	// Get userdata type for given value
	// If given value isn't userdata returns nullptr
	static const char* GetType(const LuaDynamicType& value)
	{
		if(value.GetType() != LUA_TUSERDATA) return nullptr;

		lua_State* luaVm = value.GetRef()->GetLuaVm();
		LuaStack<LuaDynamicType>::Push(luaVm, value); // Push value to stack

		// Push userdata type to stack
		// Userdata type is string(const char*),
		// but it stored in metatable as pointer(lightuserdata)
		// for fast comparsion with type from class
		lua_getfield(luaVm, -1, "_userdataType");
		if(!lua_isuserdata(luaVm, -1))
		{
			lua_pop(luaVm, 2); // Remove value and userdata type from stack
			return nullptr;
		}

		// Get userdata type from lua value
		const char *userdataType = static_cast<const char*>(lua_touserdata(luaVm, -1));

		// Remove value and userdata type from stack
		lua_pop(luaVm, 2);

		return userdataType;
	}

	// Check for userdata at given index is std::weak_ptr
	static bool IsWeak(lua_State* luaVm, int index)
	{
		lua_getuservalue(luaVm, index);

		lua_rawgeti(luaVm, -1, LUA_USERVALUE_WEAK);
		bool ret = lua_toboolean(luaVm, -1);
		lua_pop(luaVm, 2);

		return ret;
	}

	// Get raw pointer from userdata at given index
	template<typename T>
	static T* GetRawPointer(lua_State* luaVm, int index)
	{
		if(IsWeak(luaVm, index))
		{
			std::weak_ptr<T>* weakObj = *static_cast<std::weak_ptr<T>**>(lua_touserdata(luaVm, index));
			if(weakObj->expired()) return nullptr;
			else return weakObj->lock().get();
		}
		else
		{
			return *static_cast<T**>(lua_touserdata(luaVm, index));
		}
	}
};


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
	}

private:
	static IndexHandler indexHandler;

private:
	// Wrapper for constructor
	// This method is invoked when lua constructor("__call" metamethod") is called
	template<typename ... Args>
	static Class* Construct(LuaTable, Args ... args)
	{
		return new Class(args ...);
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
			Class *obj = LuaUserdata::GetRawPointer<Class>(luaVm, -2);
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

	// Handler for "__gc" metamethod
	// Calls when userdata object was collected by lua GC
	static int OnGc(lua_State* luaVm)
	{
		if(!lua_isuserdata(luaVm, 1)) return 0;

		// For weak pointers, delete only weak pointer object
		if(LuaUserdata::IsWeak(luaVm, 1))
		{
			std::weak_ptr<Class>* weakObj = *static_cast<std::weak_ptr<Class>**>(lua_touserdata(luaVm, 1));

			// Relase ref to userdata in C++ object if it's still alive
			if(!weakObj->expired())
			{
				auto ref = weakObj->lock()->_GetLuaRef();
				if(ref) ref->Release();
			}

			delete weakObj;
		}

		// For raw pointers, delete C++ object
		else
		{
			Class* userdata = *static_cast<Class**>(lua_touserdata(luaVm, 1));
			delete userdata;
		}

		return 0;
	}

public:
	template<typename ... Args>
	void SetConstructor()
	{
		lua_State *luaVm = ref->GetLuaVm();

		LuaFunction fnConstruct(luaVm);
		fnConstruct.Bind<Class*, LuaTable, Args...>(&LuaClass<Class>::Construct);

		LuaTable meta(luaVm);
		meta.SetField("__call", fnConstruct);
		SetMetatable(meta);
	}

	template<typename Ret, typename ... Args>
	void SetMethod(const std::string& name, Ret (Class::*method)(Args ...))
	{
		LuaFunction fn(ref->GetLuaVm());
		fn.Bind<Ret, Class, Args...>(method);
		SetField(name, fn);
	}

	void SetIndexHandler(IndexHandler indexHandler)
	{
		LuaClass<Class>::indexHandler = indexHandler;
	}
};

// Initialize static members of LuaClass
template<typename Class>
LuaDynamicType (Class::*LuaClass<Class>::indexHandler)(const LuaDynamicType&) = nullptr;


template<typename Class>
struct LuaStack<LuaClass<Class>>
{
	static void Push(lua_State* luaVm, LuaClass<Class> cls)
	{
		LuaStack<LuaTable>::Push(luaVm, cls);
	}
};


template<typename T>
struct LuaStack<std::weak_ptr<T>>
{
	static void Push(lua_State* luaVm, const std::weak_ptr<T>& obj)
	{
		if(obj.expired())
		{
			LUNA_LOGE("Attempt to push on lua stack expired weak_ptr");
			lua_pushnil(luaVm);
			return;
		}

		std::shared_ptr<T> sharedObj = obj.lock();
		std::shared_ptr<LuaWeakRef> ref = sharedObj->_GetLuaRef();

		// If given object already was pushed to lua
		// Just push ref to userdata on stack
		if(ref && *ref.get() != nil)
		{
			LuaStack<LuaWeakRef*>::Push(luaVm, ref.get());
		}

		else
		{
			// Make copy of weak_ptr on heap
			// This copy will be deleted by garbage collector
			// SEE: LuaClass::OnGc
			std::weak_ptr<T>* weakObj = new std::weak_ptr<T>(obj);

			std::weak_ptr<T>** userData = (std::weak_ptr<T>**)lua_newuserdata(luaVm, sizeof(std::weak_ptr<T>*));
			*userData = weakObj;

			// Set metatable to userdata object
			luaL_getmetatable(luaVm, T::_GetUserdataType());
			lua_setmetatable(luaVm, -2);

			// Create and set uservalue table to userdata
			// Uservalue table store some service parameters
			// and table to store userdata fields
			lua_createtable(luaVm, 2, 0);
			LuaStack<bool>::Push(luaVm, true);
			lua_rawseti(luaVm, -2, LUA_USERVALUE_WEAK);
			lua_setuservalue(luaVm, -2);

			// Save ref to userdata object in C++ object
			lua_pushvalue(luaVm, -1);
			sharedObj->_SetLuaRef(std::make_shared<LuaWeakRef>(luaVm));
		}
	}

	static std::weak_ptr<T> Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isuserdata(luaVm, index)) return std::weak_ptr<T>(); // Return expired weak_ptr

		// Check for given userdata is weak pointer
		if(!LuaUserdata::IsWeak(luaVm, index))
		{
			LUNA_LOGE("Cannot cast raw pointer to weak_ptr");
			return std::weak_ptr<T>();
		}

		// Check type of given userdata
		lua_getfield(luaVm, index, "_userdataType");
		if(!lua_isuserdata(luaVm, -1))
		{
			LUNA_LOGE("Userdata hasn't userdataType field");
			lua_pop(luaVm, 1); // Remove userdata type from stack
			return std::weak_ptr<T>();
		}

		const char* userdataType = static_cast<const char*>(lua_touserdata(luaVm, -1));
		const char* classType = T::_GetUserdataType();
		lua_pop(luaVm, 1); // Remove userdata type from stack

		if(userdataType != classType)
		{
			LUNA_LOGE("Cannot cast \"%s\" to \"%s\"", userdataType, classType);
			return std::weak_ptr<T>();
		}

		// Return valid weak_ptr
		std::weak_ptr<T>* weakObj = *static_cast<std::weak_ptr<T>**>(lua_touserdata(luaVm, index));
		return *weakObj;
	}
};


template<typename T>
struct LuaStack<T*>
{
	static void Push(lua_State* luaVm, T *obj)
	{
		std::shared_ptr<LuaWeakRef> ref = obj->_GetLuaRef();

		// If given object already was pushed to lua
		// Just push ref to userdata on stack
		if(ref && *ref.get() != nil)
		{
			LuaStack<LuaWeakRef*>::Push(luaVm, ref.get());
		}

		else
		{
			// Wrap given object to lua userdata object
			T** userData = (T**)lua_newuserdata(luaVm, sizeof(T*));
			*userData = obj;

			// Set metatable to userdata object
			luaL_getmetatable(luaVm, T::_GetUserdataType());
			lua_setmetatable(luaVm, -2);

			// Create and set uservalue table to userdata
			// Uservalue table store some service parameters
			// and table to store userdata fields
			lua_createtable(luaVm, 2, 0);

			LuaStack<bool>::Push(luaVm, false);
			lua_rawseti(luaVm, -2, LUA_USERVALUE_WEAK);

			lua_setuservalue(luaVm, -2);

			// Save ref to userdata object in C++ object
			lua_pushvalue(luaVm, -1);
			obj->_SetLuaRef(std::make_shared<LuaWeakRef>(luaVm));
		}
	}

	static T* Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isuserdata(luaVm, index)) return nullptr;

		// Check for given userdata is raw pointer
		if(LuaUserdata::IsWeak(luaVm, index))
		{
			LUNA_LOGE("Cannot cast weak_ptr to raw pointer");
			return nullptr;
		}

		// Get userdata type from metatable
		lua_getfield(luaVm, index, "_userdataType");
		if(!lua_isuserdata(luaVm, -1))
		{
			lua_pop(luaVm, 1); // Remove userdata type from stack
			return nullptr;
		}

		// Check type of given userdata
		const char* userdataType = static_cast<const char*>(lua_touserdata(luaVm, -1));
		const char* classType = T::_GetUserdataType();
		lua_pop(luaVm, 1); // Remove userdata type from stack

		if(userdataType != classType)
		{
			bool checkBaseClass = false;

			// If userdata type and class type are different
			// Recursively check base class types
			lua_getmetatable(luaVm, index);
			int popCount = 1;
			while(true)
			{
				popCount++;

				lua_pushliteral(luaVm, "_baseClass");
				lua_rawget(luaVm, -2);
				if(lua_isnil(luaVm, -1)) break;

				lua_getfield(luaVm, -1, "_userdataType");
				if(!lua_isuserdata(luaVm, -1))
				{
					lua_pop(luaVm, 1); // Remove base class type from stack
					break;
				}

				const char* baseClassType = static_cast<const char*>(lua_touserdata(luaVm, -1));
				lua_pop(luaVm, 1); // Remove base class type from stack

				if(baseClassType == classType)
				{
					checkBaseClass = true;
					break;
				}
			}
			lua_pop(luaVm, popCount);

			if(!checkBaseClass)
			{
				LUNA_LOGE("Cannot cast \"%s\" to \"%s\"", userdataType, classType);
				return nullptr;
			}
		}

		T* userdata = *static_cast<T**>(lua_touserdata(luaVm, index));
		return userdata;
	}
};

}
