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

#include "lunaevents.h"

using namespace luna2d;

LUNAEvents::LUNAEvents()
{
	auto lua = LUNAEngine::SharedLua();
	auto tblLuna = lua->GetGlobalTable().GetTable("luna");

	LuaTable tblEvents(lua);
	tblEvents.SetField("subscribe", LuaFunction(lua, this, &LUNAEvents::Subscribe));
	tblEvents.SetField("unsubscribeAll", LuaFunction(lua, this, &LUNAEvents::UnsubscribeAll));
	tblEvents.SetField("send", &LUNAEvents::LuaSend);

	tblLuna.SetField("events", tblEvents);
}

// Send message from Lua
// For maximize perfomance implemented as "lua_CFunction"
int LUNAEvents::LuaSend(lua_State* luaVm)
{
	auto handlersMap = LUNAEngine::SharedEvents()->handlersMap;
	const std::string& message = LuaStack<std::string>::Pop(luaVm, 1);

	auto itHandlers = handlersMap.find(message);
	if(itHandlers == handlersMap.end()) return 0;

	// Count of passed params except message name
	int paramsCount = lua_gettop(luaVm) - 1;

	// Push error handler to stack
	lua_pushcfunction(luaVm, &LuaScript::ErrorHandler);

	const auto& handlers = itHandlers->second;
	for(const auto& handler : handlers)
	{
		LuaStack<LuaFunction>::Push(luaVm, handler);

		// Copy passed params and push them after handler
		for(int i = 1; i <= paramsCount; i++) lua_pushvalue(luaVm, i + 1);

		lua_pcall(luaVm, paramsCount, 0, paramsCount + 1);
	}

	lua_pop(luaVm, 1); // Pop error handler from stack

	return 0;
}

// Subscribe given handler to message
void LUNAEvents::Subscribe(const std::string& message, const LuaFunction& handler)
{
	handlersMap[message].push_back(handler);
}

// Unsubscribe all messages for given message
void LUNAEvents::UnsubscribeAll(const std::string& message)
{
	handlersMap[message].clear();
}
