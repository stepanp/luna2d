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

#include "lunabindings.h"
#include "lunaengine.h"
#include "lunalua.h"
#include "lunalog.h"

using namespace luna2d;

// Bind common classes and functions to lua
// Bindings for some subsystems(graphics, assets, etc.) declated in subsystem constructors
// SEE: lunagraphics.cpp, lunassets.cpp
void luna2d::DoBindings()
{
	LuaScript* lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	BindLog(lua, tblLuna);
	BindUtils(lua, tblLuna);
}

// Bind "luna.log" module
void luna2d::BindLog(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblLog(lua);
	tblLuna.SetField("log", tblLog);

	// Register "info", "warning", "error" functions in "luna.log" table
	tblLog.SetField("info", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::InfoString));
	tblLog.SetField("warning", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::WarningString));
	tblLog.SetField("error", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::ErrorString));

	// Use "__call" metamethod of "luna.log" table as alias for "info" function
	// i.e. luna.log("message") = luna.log.info("message")
	LuaTable metaLog(lua);
	std::function<void(LuaNil, const std::string&)> proxy = [](LuaNil, const std::string& msg)
	{
		LUNAEngine::SharedLog()->InfoString(msg);
	};
	metaLog.SetField("__call", LuaFunction(lua, proxy));
	tblLog.SetMetatable(metaLog);

	// Register "table" function
	// "table" function for simplicity implemented as embedded script
	// SEE: "lua/scripts/logtable.lua.h"
	lua->DoString(LUNA_LUA_LOG_TABLE);
}

// Bind "luna.utilss" module
void luna2d::BindUtils(LuaScript* lua, LuaTable& tblLuna)
{
	// Register "ChanceTable" class
	lua->DoString(LUNA_CHANCE_TABLE);
}
