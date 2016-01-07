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

#include "luascript.h"

#if LUNA_PLATFORM == LUNA_PLATFORM_QT
	#include <QString>
#endif

namespace luna2d{

template<typename T>
struct LuaStack
{
	static void Push(lua_State* luaVm, const T& arg) = delete; // Push argument to lua vm
	static T Pop(lua_State* luaVm, int index = -1) = delete; // Pop argument from lua vm
};

// By default, for const references use same implementation
// as for non-reference type
template<typename T>
struct LuaStack<const T&> : public LuaStack<T> {};

template<>
struct LuaStack<int>
{
	static void Push(lua_State* luaVm, int arg)
	{
		lua_pushinteger(luaVm, arg);
	}

	static int Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isnumber(luaVm, index)) return 0;
		return lua_tointeger(luaVm, index);
	}
};

template<>
struct LuaStack<unsigned int>
{
	static void Push(lua_State* luaVm, unsigned int arg)
	{
		lua_pushinteger(luaVm, arg);
	}

	static unsigned int Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isnumber(luaVm, index)) return 0;
		return lua_tointeger(luaVm, index);
	}
};

template<>
struct LuaStack<float>
{
	static void Push(lua_State* luaVm, float arg)
	{
		lua_pushnumber(luaVm, arg);
	}

	static float Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isnumber(luaVm, index)) return 0;
		return lua_tonumber(luaVm, index);
	}
};

template<>
struct LuaStack<bool>
{
	static void Push(lua_State* luaVm, bool arg)
	{
		lua_pushboolean(luaVm, arg);
	}

	static bool Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isboolean(luaVm, index)) return false;
		return !!lua_toboolean(luaVm, index);
	}
};

template<>
struct LuaStack<std::string>
{
	static void Push(lua_State* luaVm, const std::string& arg)
	{
		lua_pushstring(luaVm, arg.c_str());
	}

	static std::string Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isstring(luaVm, index)) return "";
		return lua_tostring(luaVm, index);
	}
};

template<>
struct LuaStack<const char*>
{
	static void Push(lua_State* luaVm, const char* arg)
	{
		lua_pushstring(luaVm, arg);
	}
};

#if LUNA_PLATFORM == LUNA_PLATFORM_QT
template<>
struct LuaStack<QString>
{
	static void Push(lua_State* luaVm, const QString& arg)
	{
		lua_pushstring(luaVm, arg.toUtf8());
	}

	static QString Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_isstring(luaVm, index)) return QString("");
		return QString(lua_tostring(luaVm, index));
	}
};
#endif

template<>
struct LuaStack<lua_CFunction>
{
	static void Push(lua_State* luaVm, lua_CFunction fn)
	{
		lua_pushcclosure(luaVm, fn, 0);
	}
};

typedef void* LuaLightUserdata;

template<>
struct LuaStack<LuaLightUserdata>
{
	static void Push(lua_State* luaVm, LuaLightUserdata userdata)
	{
		lua_pushlightuserdata(luaVm, userdata);
	}
};

template<typename T>
struct LuaStack<std::vector<T>>
{
	static void Push(lua_State* luaVm, const std::vector<T>& vector)
	{
		size_t count = vector.size();
		lua_createtable(luaVm, count, 0);

		for(size_t i = 0; i < count; i++)
		{
			LuaStack<T>::Push(luaVm, vector[i]);
			lua_rawseti(luaVm, -2, i + 1/* Indexes in lua starts with 1 instead of 0 */);
		}
	}

	static std::vector<T> Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_istable(luaVm, index)) return std::vector<T>(); // Return empty vector

		int count = lua_rawlen(luaVm, index);
		std::vector<T> vector;
		vector.reserve(count);

		for(int i = 0; i < count; i++)
		{
			lua_rawgeti(luaVm, index, i + 1/* Indexes in lua starts with 1 instead of 0 */);
			vector.push_back(LuaStack<T>::Pop(luaVm, -1));
			lua_remove(luaVm, -1);
		}

		return std::move(vector);
	}
};

template<typename T>
struct LuaStack<std::unordered_map<std::string,T>>
{
	static void Push(lua_State* luaVm, const std::unordered_map<std::string,T>& map)
	{
		lua_createtable(luaVm, 0, map.size());

		for(auto& entry : map)
		{
			lua_pushstring(luaVm, entry.first.c_str());
			LuaStack<T>::Push(luaVm, entry.second);
			lua_rawset(luaVm, -3);
		}
	}
};

template<typename T>
struct LuaStack<std::unordered_set<T>>
{
	static void Push(lua_State* luaVm, const std::unordered_set<T>& set)
	{
		size_t count = set.size();
		lua_createtable(luaVm, count, 0);

		size_t i = 1; // Indexes in lua starts with 1
		for(auto& item : set)
		{
			LuaStack<T>::Push(luaVm, item);
			lua_rawseti(luaVm, -2, i);
			i++;
		}
	}
};

}
