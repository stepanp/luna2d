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

#pragma once

#include <lua.hpp>
#include "lunalog.h"
#include "lunamacro.h"

//-----------------------------
// Macro for checking lua stack
//-----------------------------
#define LUNA_CHECK_STACK(luaVm) LUNA_LOGE("%s:%d Lua stack size: %d", LUNA_FILE, __LINE__, lua_gettop(luaVm))
#define LUNA_CHECK_TYPE(luaVm, index) LUNA_LOGE("%s:%d Lua type: %s", LUNA_FILE, __LINE__, lua_typename(luaVm, lua_type(luaVm, index)))
#define LUNA_PRINT_STACK(luaVm) \
	{LUNA_LOGE("%s:%d:", LUNA_FILE, __LINE__); \
	for(int i = 1; i <= lua_gettop(luaVm); i++) { \
		LUNA_LOGE("  Lua type(%d): %s", -lua_gettop(luaVm) + i - 1, lua_typename(luaVm, lua_type(luaVm, i))); }}
#define LUNA_PRINT_TABLE(luaVm, index) \
	{LUNA_LOGE("%s:%d:", LUNA_FILE, __LINE__); \
	lua_pushvalue(luaVm, index); \
	lua_pushnil(luaVm); \
	while(lua_next(luaVm, -2) != 0){ \
		if(lua_type(luaVm, -2) == LUA_TSTRING) LUNA_LOGE("[%s] = %s", lua_tostring(luaVm, -2), lua_typename(luaVm, lua_type(luaVm, -1))); \
		else if(lua_type(luaVm, -2) == LUA_TNUMBER) LUNA_LOGE("[%d] = %s", lua_tointeger(luaVm, -2), lua_typename(luaVm, lua_type(luaVm, -1))); \
		lua_pop(luaVm, 1);} \
	lua_pop(luaVm, 1);}

namespace luna2d{

class LuaTable;

class LuaScript
{
public:
	LuaScript();
	~LuaScript();

private:
	lua_State *luaVm;
	int weakRegistryRef; // Ref to weak lua registry

private:
	static int ModuleLoader(lua_State *luaVm); // Custom module loader for load modules from assets
	void WrapDefault(); // Wrap some default lua functions
	void MakeWeakRegistry(); // Make weak table in registry for weak refs

public:
	lua_State* GetLuaVm(); // Get pointer to lua VM
	void Open(); // Open lua state
	void Close(); // Close lua state
	void DoString(const std::string& str);
	bool DoFile(const std::string& filename);
	bool LoadFile(const std::string& filename); // Load file without run
	LuaTable GetGlobalTable(); // Get global table
	int GetWeakRegistryRef();

	static int ErrorHandler(lua_State *luaVm); // Lua error handler
	static LuaScript* FromLuaVm(lua_State* luaVm); // Get pointer to LuaScript instance from lua_State

	operator lua_State*();
};

}
