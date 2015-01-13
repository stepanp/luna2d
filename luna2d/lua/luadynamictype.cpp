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

#include "luadynamictype.h"
#include "luatable.h"

using namespace luna2d;

LuaDynamicType::LuaDynamicType(const LuaNil& value) : LuaObject(value) {}
LuaDynamicType::LuaDynamicType(LuaScript* lua) : LuaObject(lua) {}
LuaDynamicType::LuaDynamicType(lua_State* luaVm) : LuaObject(luaVm) {}
LuaDynamicType::LuaDynamicType(lua_State *luaVm, int ref) : LuaObject(luaVm, ref) {}
LuaDynamicType::LuaDynamicType(const LuaDynamicType& type) : LuaObject(type) {}
LuaDynamicType::LuaDynamicType(LuaDynamicType&& type) : LuaObject(type) {}

int LuaDynamicType::GetType() const
{
	lua_State *luaVm = ref->GetLuaVm();

	lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef()); // Push variable to stack
	int type = lua_type(luaVm, -1);
	lua_pop(luaVm, 1); // Remove variable from stack

	return type;
}

LuaDynamicType& LuaDynamicType::operator=(const LuaDynamicType& type)
{
	ref = type.ref;
	return *this;
}

LuaTable LuaDynamicType::ToTable() const
{
	return To<LuaTable>();
}

LuaFunction LuaDynamicType::ToFunction() const
{
	return To<LuaFunction>();
}
