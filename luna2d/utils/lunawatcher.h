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

#include "lunalua.h"
#include <functional>
#include <unordered_map>

namespace luna2d{

struct TableHook
{
	LuaTable table = nil; /// Hooked table
	LuaFunction fnNewIndex = nil; // Hooked __newindex metamethod of table
};

class LUNAWatcherListener
{
public:
	virtual ~LUNAWatcherListener() {}
	virtual void OnTableAdded(const std::string& name) = 0;
	virtual void OnTableRemoved(const std::string& name) = 0;
	virtual void OnFieldChanged(const std::string& tableName, const std::string& fieldName, const LuaDynamicType& value) = 0;
	virtual void OnWatcherStopped() = 0;
};

class LUNAWatcher
{
public:
	~LUNAWatcher();

private:
	std::unordered_map<std::string, TableHook> hooks;
	LUNAWatcherListener* listener = nullptr;

private:
	void OnNewIndex(const LuaTable& table, const LuaDynamicType& key, const LuaDynamicType& value); // This method calls when changing any value in hooked table
	TableHook SetHook(LuaTable table, const std::string& name);
	void RemoveHook(LuaTable table);
	void FetchTable(const LuaTable& table, const std::string& name); // Fetch all fields of given table

public:
	void SetListener(LUNAWatcherListener* listener);
	void AddTable(const std::string& name, LuaTable table);
	void RemoveTable(const std::string& name);
	void SetValue(const std::string& tableName, const std::string& fieldName, LuaDynamicType value);
	void FetchAllTables(); // Force fetch field of all tables in watcher
};

}
