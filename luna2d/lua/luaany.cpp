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

#include "luaany.h"
#include "luatable.h"

using namespace luna2d;

LuaAny::LuaAny() : LuaObject() {}
LuaAny::LuaAny(const LuaNil& value) : LuaObject(value) {}
LuaAny::LuaAny(LuaScript* lua) : LuaObject(lua) {}
LuaAny::LuaAny(lua_State* luaVm) : LuaObject(luaVm) {}
LuaAny::LuaAny(lua_State *luaVm, int ref) : LuaObject(luaVm, ref) {}
LuaAny::LuaAny(const LuaAny& type) : LuaObject(type) {}

int LuaAny::GetType() const
{
	lua_State *luaVm = ref->GetLuaVm();

	lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef()); // Push variable to stack
	int type = lua_type(luaVm, -1);
	lua_pop(luaVm, 1); // Remove variable from stack

	return type;
}

LuaAny& LuaAny::operator=(const LuaAny& type)
{
	ref = type.ref;
	return *this;
}

LuaTable LuaAny::ToTable() const
{
	return To<LuaTable>();
}

LuaFunction LuaAny::ToFunction() const
{
	return To<LuaFunction>();
}
