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

using namespace luna2d;

//----------------------------------
// Helper for LuaTable::MakeReadOnly
//----------------------------------
int ReadOnlyFunc(lua_State*) { return 0; }

LuaTableIterator::LuaTableIterator() :
	tableRef(nullptr),
	keyRef(nil),
	curEntry(LuaDynamicType(nil), LuaDynamicType(nil)),
	counter(-1)
{
}

LuaTableIterator::LuaTableIterator(LuaRef* tableRef) :
	tableRef(tableRef),
	keyRef(nil),
	curEntry(LuaDynamicType(nil), LuaDynamicType(nil)),
	counter(-1)
{
	lua_State* luaVm = tableRef->GetLuaVm();

	// Set first key
	LuaStack<LuaNil>::Push(luaVm);
	keyRef.Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));

	NextIteration();
}

void LuaTableIterator::NextIteration()
{
	lua_State* luaVm = tableRef->GetLuaVm();

	LuaStack<LuaRef*>::Push(luaVm, tableRef); // Push table to stack
	LuaStack<LuaRef*>::Push(luaVm, &keyRef); // Push key to stack

	// Push next key-value pair to stack
	if(lua_next(luaVm, -2) != 0)
	{
		curEntry.first = LuaStack<LuaDynamicType>::Pop(luaVm, -2); // Get key
		curEntry.second = LuaStack<LuaDynamicType>::Pop(luaVm, -1); // Get value
		counter++;

		lua_pop(luaVm, 1); // Pop value from stack

		// Hold ref to current key for next iteration
		keyRef.Release();
		keyRef.Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	}
	else counter = -1;

	lua_pop(luaVm, 1); // Pop table from stack
}

bool LuaTableIterator::operator==(const LuaTableIterator& iterator)
{
	return counter == iterator.counter;
}

bool LuaTableIterator::operator!=(const LuaTableIterator& iterator)
{
	return counter != iterator.counter;
}

LuaTableIterator& LuaTableIterator::operator++()
{
	NextIteration();
	return *this;
}

std::pair<LuaDynamicType,LuaDynamicType>& LuaTableIterator::operator*()
{
	return curEntry;
}


LuaTable::LuaTable(const LuaNil& value) : LuaObject(value) {}
LuaTable::LuaTable(LuaScript* lua) : LuaTable(lua->GetLuaVm()) {}

LuaTable::LuaTable(lua_State* luaVm) : LuaObject(luaVm)
{
	lua_createtable(luaVm, 0, 0);
	ref->Hold(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
}

LuaTable::LuaTable(lua_State* luaVm, int ref) : LuaObject(luaVm, ref) {}
LuaTable::LuaTable(const LuaTable& table) : LuaObject(table) {}

// Get metatable of this table. If table hasn't metatable return nil table
LuaTable LuaTable::GetMetatable() const
{
	lua_State *luaVm = ref->GetLuaVm();

	LuaStack<LuaTable>::Push(luaVm, *this); // Push this table to stack
	if(lua_getmetatable(luaVm, -1))
	{
		LuaTable&& ret = LuaStack<LuaTable>::Pop(luaVm, -1); // Get metatable from stack
		lua_pop(luaVm, 2); // Remove this table and metatable from stack
		return ret;
	}

	lua_pop(luaVm, 1); // Remove this table from stack
	return nil; // Return nil table
}

// Set given table as metatable for this table
void LuaTable::SetMetatable(const LuaTable& meta)
{
	lua_State *luaVm = ref->GetLuaVm();

	LuaStack<LuaTable>::Push(luaVm, *this); // Push this table to stack
	LuaStack<LuaTable>::Push(luaVm, meta);
	lua_setmetatable(luaVm, -2);

	lua_pop(luaVm, 1); // Remove this table from stack
}

// Check for field with given key is exists(not nil)
bool LuaTable::HasField(const std::string& name, bool rawMode) const
{
	lua_State *luaVm = ref->GetLuaVm();

	LuaStack<LuaTable>::Push(luaVm, *this); // Push this table to stack
	lua_pushstring(luaVm, name.c_str());
	rawMode ? lua_rawget(luaVm, -2) : lua_gettable(luaVm, -2);
	bool ret = lua_isnil(luaVm, -1) == 0;

	lua_pop(luaVm, 2); // Remove this table and field from stack

	return ret;
}

void LuaTable::Clear()
{
	lua_State *luaVm = ref->GetLuaVm();

	// Push this table and first key on stack
	LuaStack<LuaTable>::Push(luaVm, *this);
	LuaStack<LuaNil>::Push(luaVm); // Nil - is always first key

	// lua_next pushes on stack:
	// key at index -2
	// value at index -1
	while(lua_next(luaVm, -2) != 0)
	{
		// Replace current value with nil(i.e. remove it)
		lua_remove(luaVm, -1);
		lua_pushvalue(luaVm, -1);
		lua_pushnil(luaVm);
		lua_rawset(luaVm, -4);
	}

	lua_pop(luaVm, 2); // Remove this table and key from stack
}

// Check for table has at least on field
bool LuaTable::IsEmpty() const
{
	lua_State *luaVm = ref->GetLuaVm();

	// Push this table and first key on stack
	LuaStack<LuaTable>::Push(luaVm, *this);
	LuaStack<LuaNil>::Push(luaVm); // Nil - is always first key

	bool empty = lua_next(luaVm, -2) == 0;
	lua_pop(luaVm, empty ? 1 : 3); // Remove table, key and value from stack

	return empty;
}

 // Get count of items for array table
int LuaTable::GetArrayCount() const
{
	lua_State *luaVm = ref->GetLuaVm();

	LuaStack<LuaTable>::Push(luaVm, *this);
	int ret = lua_rawlen(luaVm, -1);
	lua_pop(luaVm, 1);

	return ret;
}

// Deny modify table from lua
void LuaTable::MakeReadOnly()
{
	LuaTable meta = GetMetatable();
	if(meta == nil) meta = LuaTable(ref->GetLuaVm());

	meta.SetField("__newindex", &ReadOnlyFunc);

	SetMetatable(meta);
}

LuaTableIterator LuaTable::begin() const
{
	if(*this == nil) return end();
	return LuaTableIterator(ref.get());
}

LuaTableIterator LuaTable::end() const
{
	return LuaTableIterator();
}

LuaTable& LuaTable::operator=(const LuaTable& table)
{
	ref = table.ref;
	return *this;
}
