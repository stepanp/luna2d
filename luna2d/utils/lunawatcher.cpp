//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunawatcher.h"
#include "lunaengine.h"
#include <utility>

using namespace luna2d;

LUNAWatcher::~LUNAWatcher()
{
	if(listener) listener->OnWatcherStopped();
}

// This method calls when changing any value in hooked table
void LUNAWatcher::OnNewIndex(const LuaTable& table, const LuaDynamicType& key, const LuaDynamicType& value)
{
	std::string tableName = table.GetMetatable().GetString("__watcherId");

	if(tableName.length() > 0 && listener && key.GetType() == LUA_TSTRING)
	{
		listener->OnFieldChanged(tableName, key.ToString(), value);
	}
}

TableHook LUNAWatcher::SetHook(LuaTable table, const std::string& name)
{
	LuaTable meta = table.GetMetatable();

	// Make empty metatable if table hasn't metatable
	if(meta == nil)
	{
		 meta = LuaTable(LUNAEngine::SharedLua());
		 table.SetMetatable(meta);
	}

	// Save hook params
	TableHook hook;
	hook.table = table;
	hook.fnNewIndex = meta.GetFunction("__newindex");

	meta.SetField("__newindex", LuaFunction(LUNAEngine::SharedLua(), this, &LUNAWatcher::OnNewIndex));
	meta.SetField("__watcherId", name);

	// Fetch all fields of table
	FetchTable(table, name);

	return hook;
}

void LUNAWatcher::RemoveHook(LuaTable table)
{

}

 // Fetch all fields of given table
void LUNAWatcher::FetchTable(const LuaTable& table, const std::string &name)
{
	if(!listener) return;

	for(auto entry : table)
	{
		if(entry.first.GetType() == LUA_TSTRING)
		{
			listener->OnFieldChanged(name, entry.first.ToString(), entry.second);
		}
	}
}

void LUNAWatcher::SetListener(LUNAWatcherListener* listener)
{
	this->listener = listener;
}

void LUNAWatcher::AddTable(const std::string& name, LuaTable table)
{
	if(hooks.count(name) > 0)
	{
		LUNA_LOGE("Cannot add table \"%s\" to watcher. Because table with same name already in watcher", name.c_str());
		return;
	}

	if(table == nil)
	{
		LUNA_LOGE("Cannot add table \"%s\" to watcher. Wrong table", name.c_str());
		return;
	}

	hooks[name] = SetHook(table, name);
	if(listener) listener->OnTableAdded(name);
}

void LUNAWatcher::RemoveTable(const std::string &name)
{
	/*if(tables.count(name) > 0)
	{
		auto it = tables.find(name);
		RemoveHook(it->second);
		tables.erase(it);
	}*/

	if(listener) listener->OnTableRemoved(name);
}

void LUNAWatcher::SetValue(const std::string& tableName, const std::string& fieldName, LuaDynamicType value)
{
	if(hooks.count(tableName) > 0)
	{
		TableHook& hook = hooks[tableName];
		hook.table.SetField(fieldName.c_str(), value);
	}
}

// Force fetch field of all tables in watcher
void LUNAWatcher::FetchAllTables()
{
	for(auto entry : hooks) FetchTable(entry.second.table, entry.first);
}
