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
#include "luastack.h"
#include "luanil.h"

namespace luna2d{

//--------------------
// Holder for lua refs
//--------------------
class LuaRef
{
public:
	LuaRef(const LuaNil&);
	LuaRef(LuaScript* lua);
	LuaRef(lua_State* luaVm);
	LuaRef(lua_State* luaVm, int ref);
	~LuaRef();

protected:
	lua_State* luaVm;
	int ref;

public:
	void Hold(lua_State* luaVm, int ref);
	void Release();
	int GetRef() const;
	lua_State* GetLuaVm() const;
	bool operator==(const LuaNil&) const;
	bool operator!=(const LuaNil&) const;
	operator bool();
};


//-------------------------
// Holder for lua weak refs
//-------------------------
class LuaWeakRef
{
public:
	LuaWeakRef(const LuaNil&);
	LuaWeakRef(LuaScript* lua);
	LuaWeakRef(lua_State* luaVm);
	~LuaWeakRef();

protected:
	lua_State* luaVm;
	int ref; // Here, ref is index in special weak table in lua registry

public:
	void Hold(lua_State* luaVm); // Hold value from top of lua stack
	void Release();
	void Validate(); // Check and release ref if referenced object already deleted
	int GetRef() const;
	lua_State* GetLuaVm() const;
	bool operator==(const LuaNil&) const;
	bool operator!=(const LuaNil&) const;
	operator bool();
};


template<>
struct LuaStack<LuaRef*>
{
	static void Push(lua_State* luaVm, LuaRef* ref)
	{
		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
	}
};

template<>
struct LuaStack<LuaWeakRef*>
{
	static void Push(lua_State* luaVm, LuaWeakRef* ref)
	{
		if(ref->GetRef() == LUA_NOREF)
		{
			lua_pushnil(luaVm);
			return;
		}

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, LuaScript::FromLuaVm(luaVm)->GetWeakRegistryRef());
		lua_rawgeti(luaVm, -1, ref->GetRef());
		lua_remove(luaVm, -2); // Remove weak registry table from stack
	}
};

}
