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

#include "luaref.h"

namespace luna2d{

template<typename T>
struct LuaStack<std::shared_ptr<T>>
{
	static void Push(lua_State* luaVm, const std::shared_ptr<T>& ptr)
	{
		if(!ptr)
		{
			LuaStack<LuaNil>::Push(luaVm);
			return;
		}

		std::shared_ptr<LuaWeakRef> ref = ptr->_GetLuaRef();

		// If given object already was pushed to Lua
		// Just push ref to userdata on stack
		if(ref && *ref.get() != nil)
		{
			LuaStack<LuaWeakRef*>::Push(luaVm, ref.get());
		}
		else
		{
			// Make copy of shared_ptr on heap
			// This copy can be deleted by garbage collector
			// SEE: LuaClass::OnGc
			std::shared_ptr<T>* heapPtr = new std::shared_ptr<T>(ptr);

			std::shared_ptr<T>** userData = (std::shared_ptr<T>**)lua_newuserdata(luaVm, sizeof(std::shared_ptr<T>*));
			*userData = heapPtr;

			// Set metatable to userdata object
			luaL_getmetatable(luaVm, T::_GetTypeName());
			lua_setmetatable(luaVm, -2);

			// Save ref to userdata object in C++ object
			lua_pushvalue(luaVm, -1);
			ptr->_SetLuaRef(std::make_shared<LuaWeakRef>(luaVm));
		}
	}

	static std::shared_ptr<T> Pop(lua_State* luaVm, int index)
	{
		if(!lua_isuserdata(luaVm, index)) return nullptr;

		// Get userdata type from metatable
		lua_getfield(luaVm, index, "_typeId");
		if(!lua_isnumber(luaVm, -1))
		{
			lua_pop(luaVm, 1); // Remove userdata type from stack
			return nullptr;
		}

		int userdataType = lua_tointeger(luaVm, -1);
		int classType = T::_GetTypeId();
		lua_pop(luaVm, 1); // Remove userdata type from stack

		// If userdata type and class type are different
		// Recursively check base class types
		if(userdataType != classType)
		{
			bool checkBaseClass = false;
			int popCount = 1;

			lua_getmetatable(luaVm, index);
			while(true)
			{
				popCount++;

				lua_pushliteral(luaVm, "_baseClass");
				lua_rawget(luaVm, -2);
				if(lua_isnil(luaVm, -1)) break;

				lua_getfield(luaVm, -1, "_userdataType");
				if(!lua_isnumber(luaVm, -1))
				{
					lua_pop(luaVm, 1); // Remove base class type from stack
					break;
				}

				int baseClassType = lua_tointeger(luaVm, -1);
				lua_pop(luaVm, 1); // Remove base class type from stack

				if(baseClassType == classType)
				{
					checkBaseClass = true;
					break;
				}
			}
			lua_pop(luaVm, popCount);

			if(!checkBaseClass) return nullptr;
		}

		// Return valid shared_ptr
		std::shared_ptr<T>* ptr = *static_cast<std::shared_ptr<T>**>(lua_touserdata(luaVm, index));
		return *ptr;
	}
};

// Helper to implicit casting between std::weak_ptr and std::shared_ptr when working with lua stack
template<typename T>
struct LuaStack<std::weak_ptr<T>>
{
	// "std::weak_ptr" cannot be pushed directly to lua
	// Instead, original "std::shared_ptr" will pushed
	// If ptr is expired, nil will pushed
	static void Push(lua_State* luaVm, const std::weak_ptr<T>& ptr)
	{
		return LuaStack<std::shared_ptr<T>>::Push(luaVm, ptr.lock());
	}

	static std::weak_ptr<T> Pop(lua_State* luaVm, int index)
	{
		return std::weak_ptr<T>(LuaStack<std::shared_ptr<T>>::Pop(luaVm, index));
	}
};

}
