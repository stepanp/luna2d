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

#include "luafunction.h"
#include "luadynamictype.h"
#include <iterator>

namespace luna2d{

class LuaTableIterator : public std::iterator<std::forward_iterator_tag, std::pair<LuaDynamicType,LuaDynamicType>>
{
public:
	LuaTableIterator();
	LuaTableIterator(LuaRef* tableRef);

private:
	LuaRef* tableRef;
	LuaRef keyRef;
	std::pair<LuaDynamicType,LuaDynamicType> curEntry;
	int counter;

private:
	void NextIteration();

public:
	bool operator==(const LuaTableIterator& iterator);
	bool operator!=(const LuaTableIterator& iterator);
	LuaTableIterator& operator++();
	std::pair<LuaDynamicType,LuaDynamicType>& operator*();
};


class LuaTable : public LuaObject
{
public:
	LuaTable(const LuaNil& value);
	LuaTable(LuaScript* lua);
	LuaTable(lua_State* luaVm);
	LuaTable(lua_State* luaVm, int ref);
	LuaTable(const LuaTable& table);
	LuaTable(LuaTable&& table);

public:
	LuaTable GetMetatable() const; // Get metatable of this table. If table hasn't metatable return nil table
	void SetMetatable(const LuaTable& meta); // Set given table as metatable for this table
	bool HasField(const std::string& name) const; // Check for field with given key is exists(not nil)
	void Clear(); // Remove all fields from table
	bool IsEmpty(); // Check for table has at least on field
	void MakeReadOnly(); // Deny modify table from lua

	// Get field of table by string key
	template<typename Ret>
	Ret GetField(const std::string& name) const
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		lua_getfield(luaVm, -1, name.c_str());

		Ret&& ret = LuaStack<Ret>::Pop(luaVm, -1);
		lua_pop(luaVm, 2); // Remove table and field from stack

		return ret;
	}

	// Get field of table by integer key
	template<typename Ret>
	Ret GetArrayField(int index) const
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		lua_rawgeti(luaVm, -1, index);

		Ret&& ret = LuaStack<Ret>::Pop(luaVm, -1);
		lua_pop(luaVm, 2); // Remove table and field from stack

		return ret;
	}

	// Set field of table by string key
	template<typename Arg>
	void SetField(const std::string& name, Arg arg)
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		LuaStack<Arg>::Push(luaVm, arg);
		lua_setfield(luaVm, -2, name.c_str());

		// Remove table from stack
		lua_pop(luaVm, 1);
	}

	// Set field of table by integer key
	template<typename Arg>
	void SetArrayField(int index, Arg arg)
	{
		lua_State *luaVm = ref->GetLuaVm();

		lua_rawgeti(luaVm, LUA_REGISTRYINDEX, ref->GetRef());
		LuaStack<Arg>::Push(luaVm, arg);
		lua_rawseti(luaVm, -2, index);

		// Remove table from stack
		lua_pop(luaVm, 1);
	}

	// Iterators for foreach cycle
	LuaTableIterator begin() const;
	LuaTableIterator end() const;

	LuaTable& operator=(const LuaTable& table);

	inline void RemoveField(const std::string& name) { SetField(name, nil); }
	inline void RemoveArrayField(int index) { SetArrayField(index, nil); }

	// Helpers for getting fields with most frequently used types
	// without having to specify template of return type
	inline LuaFunction GetFunction(const std::string& name) const { return GetField<LuaFunction>(name); }
	inline LuaTable GetTable(const std::string& name) const { return GetField<LuaTable>(name); }
	inline bool GetBool(const std::string& name) const { return GetField<bool>(name); }
	inline int GetInt(const std::string& name) const { return GetField<int>(name); }
	inline float GetFloat(const std::string& name) const { return GetField<float>(name); }
	inline std::string GetString(const std::string& name) const { return GetField<std::string>(name); }
};


template<>
struct LuaStack<LuaTable>
{
	static void Push(lua_State* luaVm, const LuaTable& table)
	{
		LuaStack<LuaRef*>::Push(luaVm, table.GetRef().get());
	}

	static LuaTable Pop(lua_State* luaVm, int index = -1)
	{
		if(!lua_istable(luaVm, index)) return std::move(LuaTable(luaVm, LUA_NOREF)); // Return nil table

		// Push table from index to top of stack
		// because "luaL_ref" can get ref only from top of stack
		lua_pushvalue(luaVm, index);

		int ref = luaL_ref(luaVm, LUA_REGISTRYINDEX);
		return LuaTable(luaVm, ref);
	}
};

}
