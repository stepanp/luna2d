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

#include "luatable.h"
#include "luascript.h"

using namespace luna2d;

LuaRef::LuaRef(const LuaNil&)
{
	luaVm = nullptr;
	ref = LUA_NOREF;
}

LuaRef::LuaRef(LuaScript* lua)
{
	Hold(lua->GetLuaVm(), LUA_NOREF);
}

LuaRef::LuaRef(lua_State* luaVm)
{
	Hold(luaVm, LUA_NOREF);
}

LuaRef::LuaRef(lua_State* luaVm, int ref)
{
	Hold(luaVm, ref);
}

LuaRef::~LuaRef()
{
	Release();
}

void LuaRef::Hold(lua_State* luaVm, int ref)
{
	this->luaVm = luaVm;
	this->ref = ref;
}

void LuaRef::Release()
{
	if(ref != LUA_NOREF)
	{
		luaL_unref(luaVm, LUA_REGISTRYINDEX, ref);
		ref = LUA_NOREF;
	}
}

int LuaRef::GetRef() const
{
	return ref;
}

lua_State* LuaRef::GetLuaVm() const
{
	return luaVm;
}

bool LuaRef::operator==(const LuaNil&) const
{
	return ref == LUA_NOREF;
}

bool LuaRef::operator!=(const LuaNil&) const
{
	return ref != LUA_NOREF;
}

LuaRef::operator bool()
{
	return ref != LUA_NOREF;
}


LuaWeakRef::LuaWeakRef(const LuaNil&) :
	luaVm(nullptr),
	ref(LUA_NOREF)
{
}

LuaWeakRef::LuaWeakRef(LuaScript* lua)
{
	Hold(lua->GetLuaVm());
}

LuaWeakRef::LuaWeakRef(lua_State* luaVm)
{
	Hold(luaVm);
}

LuaWeakRef::~LuaWeakRef()
{
	Release();
}

void LuaWeakRef::Hold(lua_State* luaVm)
{
	this->luaVm = luaVm;

	// Get weak registry table
	lua_rawgeti(luaVm, LUA_REGISTRYINDEX, LuaScript::FromLuaVm(luaVm)->GetWeakRegistryRef());

	// Add value from top of stack to weak registry
	ref = lua_rawlen(luaVm, -1) + 1;
	lua_insert(luaVm, lua_gettop(luaVm) -1); // Swap weak registry table and holding value in stack
	lua_rawseti(luaVm, -2, ref);

	lua_pop(luaVm, 1); // Remove weak registry table and holded value from stack
}

void LuaWeakRef::Release()
{
	if(ref != LUA_NOREF)
	{
		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, LuaScript::FromLuaVm(luaVm)->GetWeakRegistryRef());
		lua_pushnil(luaVm);
		lua_rawseti(luaVm, -2, ref);
		lua_pop(luaVm, 1); // Remove weak registry table

		ref = LUA_NOREF;
	}
}

int LuaWeakRef::GetRef() const
{
	return ref;
}

lua_State* LuaWeakRef::GetLuaVm() const
{
	return luaVm;
}

bool LuaWeakRef::operator==(const LuaNil&) const
{
	return ref == LUA_NOREF;
}

bool LuaWeakRef::operator!=(const LuaNil&) const
{
	return ref != LUA_NOREF;
}

LuaWeakRef::operator bool()
{
	return ref != LUA_NOREF;
}

