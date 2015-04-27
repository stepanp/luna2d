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

#include "luainstance.h"

using namespace luna2d;

LuaInstance::LuaInstance() : LuaObject() {}
LuaInstance::LuaInstance(const LuaNil& value) : LuaObject(value) {}
LuaInstance::LuaInstance(LuaScript* lua) : LuaObject(lua) {}
LuaInstance::LuaInstance(lua_State* luaVm) : LuaObject(luaVm) {}
LuaInstance::LuaInstance(lua_State* luaVm, int ref, bool isUserdata) : LuaObject(luaVm, ref), isUserdata(isUserdata) {}
LuaInstance::LuaInstance(const LuaInstance& instance) : LuaObject(instance) {}

bool LuaInstance::IsUserdata() const
{
	return isUserdata;
}

bool LuaInstance::HasField(const std::string& name) const
{
	lua_State* luaVm = ref->GetLuaVm();

	LuaStack<LuaRef*>::Push(luaVm, ref.get());
	lua_getfield(luaVm, -1, name.c_str());
	bool ret = lua_isnil(luaVm, -1) == 0;
	lua_pop(luaVm, 1);

	return ret;
}

LuaFunction LuaInstance::GetFunction(const std::string& name) const
{
	lua_State* luaVm = ref->GetLuaVm();

	LuaStack<LuaRef*>::Push(luaVm, ref.get());
	lua_getfield(luaVm, -1, name.c_str());
	LuaFunction fn = LuaStack<LuaFunction>::Pop(luaVm, -1);
	lua_pop(luaVm, 1);

	return fn;
}

LuaTable LuaInstance::ToTable() const
{
	return LuaTable(ref->GetLuaVm(), ref->GetRef());
}

