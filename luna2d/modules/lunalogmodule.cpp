//---------------------------------
// luna2d engine
// lunalogmodule.cpp - Log module
// Copyright 2014 Stepan Prokofjev
//---------------------------------

#include "lunalogmodule.h"
#include "lunalog.h"

using namespace luna2d;

// Proxy for "__call" metametod
// First param of "__call" metamethod is table
// Because, we need use this proxy to filter this param
void LUNALogModule::CallProxy(LuaTable, const char* message)
{
	LUNA_LOG(message);
}

void LUNALogModule::Load(LuaScript *lua)
{
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblLog(lua);

	// Register "info", "warning", "error" functions in "luna.log" table
	tblLog.SetField("info", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::InfoNoVa));
	tblLog.SetField("warning", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::WarningNoVa));
	tblLog.SetField("error", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::ErrorNoVa));

	// Use "__call" metamethod of "luna.log" table as alias for "info" function
	// i.e. luna.log("message") = luna.log.info("message")
	LuaTable metaLog(lua);
	metaLog.SetField("__call", LuaFunction(lua, this, &LUNALogModule::CallProxy));
	tblLog.SetMetatable(metaLog);

	tblLuna.SetField("log", tblLog);

	// Register "table" function
	// "table" function for simplicity implemented as embedded script
	// SEE: "lua/scripts/logtable.lua.h"
	lua->DoString(LUNA_LUA_LOG_TABLE);
}
