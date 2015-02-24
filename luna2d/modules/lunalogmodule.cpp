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

#include "lunalogmodule.h"
#include "lunalog.h"

using namespace luna2d;

// Proxy for "__call" metametod
// First param of "__call" metamethod is table
// Because, we need use this proxy to filter this param
void CallProxy(LuaTable, const std::string& message)
{
	LUNAEngine::SharedLog()->InfoString(message);
}

void LUNALogModule::Load(LuaScript *lua)
{
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblLog(lua);

	// Register "info", "warning", "error" functions in "luna.log" table
	tblLog.SetField("info", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::InfoString));
	tblLog.SetField("warning", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::WarningString));
	tblLog.SetField("error", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::ErrorString));

	// Use "__call" metamethod of "luna.log" table as alias for "info" function
	// i.e. luna.log("message") = luna.log.info("message")
	LuaTable metaLog(lua);
	metaLog.SetField("__call", LuaFunction(lua, &CallProxy));
	tblLog.SetMetatable(metaLog);

	tblLuna.SetField("log", tblLog);

	// Register "table" function
	// "table" function for simplicity implemented as embedded script
	// SEE: "lua/scripts/logtable.lua.h"
	lua->DoString(LUNA_LUA_LOG_TABLE);
}
