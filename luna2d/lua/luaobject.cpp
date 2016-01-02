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

#include "luaobject.h"

using namespace luna2d;

LuaObject::LuaObject() : LuaObject(nil)
{
}

LuaObject::LuaObject(const LuaNil& value) :
	ref(std::make_shared<LuaRef>(value))
{
}

LuaObject::LuaObject(LuaScript* lua) :
	ref(std::make_shared<LuaRef>(lua))
{
}

LuaObject::LuaObject(lua_State* luaVm) :
	ref(std::make_shared<LuaRef>(luaVm))
{
}

LuaObject::LuaObject(lua_State* luaVm, int ref) :
	ref(std::make_shared<LuaRef>(luaVm, ref))
{
}

LuaObject::LuaObject(const LuaObject& obj) :
	ref(obj.ref)
{
}

std::shared_ptr<LuaRef> LuaObject::GetRef() const
{
	return ref;
}

bool LuaObject::operator==(const LuaNil&) const
{
	return *ref.get() == nil;
}

bool LuaObject::operator!=(const LuaNil&) const
{
	return *ref.get() != nil;
}

LuaObject::operator bool() const
{
	return *ref.get() != nil;
}

LuaObject LuaObject::operator=(const LuaObject& obj)
{
	ref = obj.ref;
	return *this;
}
