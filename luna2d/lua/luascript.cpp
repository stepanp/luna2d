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

#include "luascript.h"
#include "luatable.h"
#include "lunaengine.h"
#include "lunafiles.h"

using namespace luna2d;

static const bool IS_64BIT_ARCH = sizeof(size_t) == 8; // Is current binary built for 64-bit architecture

LuaScript::LuaScript()
{
	Open();
}

LuaScript::~LuaScript()
{
	Close();
}

// Custom module loader for load modules from assets
int LuaScript::ModuleLoader(lua_State *luaVm)
{
	LuaScript* lua = LuaScript::FromLuaVm(luaVm);
	std::string moduleName = LuaStack<std::string>::Pop(luaVm, 1) + ".lua";

	// Get path to script file where was called "require" function
	lua_Debug info;
	lua_getstack(luaVm, 2, &info);
	lua_getinfo(luaVm, "S", &info);
	std::string sourcePath = LUNAEngine::SharedFiles()->GetParentPath(info.source) + "/";

	// Try load module by relative path
	if(lua->LoadFile(sourcePath + moduleName)) return 1;

	// Try load module by global path
	if(lua->LoadFile("scripts/" + moduleName)) return 1;

	return 0;
}

// Wrap some default lua functions
void LuaScript::WrapDefault()
{
	// Wrap module loader for load modules from assets
	LuaTable searchers = GetGlobalTable().GetTable("package").GetTable("searchers");

	lua_rawgeti(luaVm, LUA_REGISTRYINDEX, searchers.GetRef()->GetRef());
	LuaStack<lua_CFunction>::Push(luaVm, &LuaScript::ModuleLoader);
	lua_rawseti(luaVm, -2, 2);
	lua_pop(luaVm, 1);
}

void LuaScript::MakeWeakRegistry()
{
	lua_createtable(luaVm, 0, 0);
	lua_createtable(luaVm, 0, 1);
	lua_pushliteral(luaVm, "__mode");
	lua_pushliteral(luaVm, "v");
	lua_rawset(luaVm, -3);
	lua_setmetatable(luaVm, -2);

	weakRegistryRef = luaL_ref(luaVm, LUA_REGISTRYINDEX);
}

// Get pointer to lua VM
lua_State* LuaScript::GetLuaVm()
{
	return luaVm;
}

// Open lua state
void LuaScript::Open()
{
	luaVm = luaL_newstate();
	luaL_openlibs(luaVm); // Open standard lua libs
	WrapDefault(); // Wrap default functions
	MakeWeakRegistry();

	// Store pointer to LuaScript instance in lua registry
	lua_pushliteral(luaVm, "_L");
	lua_pushlightuserdata(luaVm, this);
	lua_rawset(luaVm, LUA_REGISTRYINDEX);
}

// Close lua state
void LuaScript::Close()
{
	lua_close(luaVm);
}

void LuaScript::DoString(const std::string& str)
{
	lua_pushcfunction(luaVm, &LuaScript::ErrorHandler); // Set error handler
	luaL_loadstring(luaVm, str.c_str());
	lua_pcall(luaVm, 0, LUA_MULTRET, -2); // Call with using eror handler
}

bool LuaScript::DoFile(const std::string& filename)
{
	lua_pushcfunction(luaVm, &LuaScript::ErrorHandler); // Set error handler
	if(!LoadFile(filename))
	{
		lua_pop(luaVm, 1); // Remove error handler from stack
		return false;
	}

	lua_pcall(luaVm, 0, LUA_MULTRET, -2); // Call with using error handler
	return true;
}

// Load file without run
bool LuaScript::LoadFile(const std::string& filename)
{
	// On 64-bit platforms try load file with 64-bit bytecode version if it exists
	bool use64bit = IS_64BIT_ARCH && LUNAEngine::SharedFiles()->IsExists(filename + "64");

	// "luaL_dofile" cannot open file from assets(e.g. in .apk)
	// Because load file as buffer and do file using "luaL_loadbuffer"
	std::string buffer = LUNAEngine::SharedFiles()->ReadFileToString(use64bit ? filename + "64" : filename);
	if(buffer.empty()) return false;

	luaL_loadbuffer(luaVm, buffer.c_str(), buffer.size(), filename.c_str());

	return true;
}

// Get global table
LuaTable LuaScript::GetGlobalTable()
{
	lua_pushglobaltable(luaVm);
	int ref = luaL_ref(luaVm, LUA_REGISTRYINDEX);

	return LuaTable(luaVm, ref);
}

int LuaScript::GetWeakRegistryRef()
{
	return weakRegistryRef;
}

// Lua error handler
int LuaScript::ErrorHandler(lua_State *luaVm)
{
	// Log error
	LUNA_LOGE("%s", lua_tostring(luaVm, 1));

	// Log stack trace
	LUNA_LOGE("Stack trace:");
	lua_Debug info;
	int depth = 0;
	while(lua_getstack(luaVm, depth, &info))
	{
		lua_getinfo(luaVm, "Sln", &info);

		if(info.currentline > -1) // Skip lines with native lua functions
		{
			LUNA_LOGE("%s:%d: %s\n", info.source, info.currentline, info.name ? info.name : "");
		}

		depth++;
	}

	return 0;
}

// Get pointer to LuaScript instance from lua_State
LuaScript* LuaScript::FromLuaVm(lua_State* luaVm)
{
	lua_pushliteral(luaVm, "_L");
	lua_rawget(luaVm, LUA_REGISTRYINDEX);

	LuaScript* lua = static_cast<LuaScript*>(lua_touserdata(luaVm, -1));
	lua_pop(luaVm, 1); // Remove pointer from stack

	return lua;
}

LuaScript::operator lua_State*()
{
	return GetLuaVm();
}
