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

#define RUN_DELAYED_ACTION(FUNCTION_CALL) \
	if(processing) delayedActions.push_back([=](){ FUNCTION_CALL; }); \
	else FUNCTION_CALL

using namespace luna2d;

LUNAEvents::LUNAEvents()
{
	auto lua = LUNAEngine::SharedLua();
	auto tblLuna = lua->GetGlobalTable().GetTable("luna");

	LuaTable tblEvents(lua);
	tblEvents.SetField("subscribe", LuaFunction(lua, this, &LUNAEvents::Subscribe));
	tblEvents.SetField("unsubscribe", LuaFunction(lua, this, &LUNAEvents::Unsubscribe));
	tblEvents.SetField("unsubscribeAll", LuaFunction(lua, this, &LUNAEvents::UnsubscribeAll));
	tblEvents.SetField("subscribeCustom", LuaFunction(lua, this, &LUNAEvents::SubscribeCustom));
	tblEvents.SetField("unsubscribeCustom", LuaFunction(lua, this, &LUNAEvents::UnsubscribeCustom));
	tblEvents.SetField("unsubscribeAllCustom", LuaFunction(lua, this, &LUNAEvents::UnsubscribeAllCustom));
	tblEvents.SetField("send", &LUNAEvents::LuaSend);

	tblLuna.SetField("events", tblEvents);
}

// Send message from Lua
// For maximize perfomance implemented as "lua_CFunction"
int LUNAEvents::LuaSend(lua_State* luaVm)
{
	auto events = LUNAEngine::SharedEvents();
	const std::string& message = LuaStack<std::string>::Pop(luaVm, 1);

	// Events is currently processing
	bool nestedSend = events->processing;
	events->processing = true;

	// Count of passed params
	int paramsCount = lua_gettop(luaVm);

	// Push error handler to stack
	lua_pushcfunction(luaVm, &LuaScript::ErrorHandler);

	// Run custom handlers (runs for each message)
	for(const auto& customHandler : events->customHandlers)
	{
		LuaStack<LuaFunction>::Push(luaVm, customHandler);

		// Call function with all passed params
		for(int i = 1; i <= paramsCount; i++) lua_pushvalue(luaVm, i);
		lua_pcall(luaVm, paramsCount, 0, -(paramsCount + 2));
	}

	// Run handlers for given message if exists
	auto itHandlers = events->handlersMap.find(message);
	if(itHandlers != events->handlersMap.end())
	{
		for(const auto& handler : itHandlers->second)
		{
			LuaStack<LuaFunction>::Push(luaVm, handler);

			// Call function with all passed params except message name
			for(int i = 2; i <= paramsCount; i++) lua_pushvalue(luaVm, i);
			lua_pcall(luaVm, paramsCount - 1, 0, -(paramsCount + 1));
		}
	}

	lua_pop(luaVm, 1); // Pop error handler from stack

	// If it's root send, finish events processing
	if(!nestedSend)
	{
		events->processing = false;
		events->ProcessDelayedActions();
	}

	return 0;
}

void LUNAEvents::ProcessDelayedActions()
{
	for(const auto& action : delayedActions) action();
	delayedActions.clear();
}

void LUNAEvents::DoSubscribe(const std::string& message, const LuaFunction& handler)
{
	handlersMap[message].push_back(handler);
}

void LUNAEvents::DoUnsubscribe(const std::string& message, const LuaFunction& handler)
{
	auto it = handlersMap.find(message);
	if(it == handlersMap.end()) LUNA_RETURN_ERR("Cannot unsubscribe event. Message \"%s\" not found", message.c_str());

	auto& handlers = handlersMap[message];

	auto findIt = std::find(handlers.begin(), handlers.end(), handler);
	if(findIt != handlers.end()) handlers.erase(findIt);
}

void LUNAEvents::DoUnsubscribeAll(const std::string& message)
{
	handlersMap[message].clear();
}

void LUNAEvents::DoSubscribeCustom(const LuaFunction& handler)
{
	customHandlers.push_back(handler);
}

void LUNAEvents::DoUnsubscribeCustom(const LuaFunction& handler)
{
	auto findIt = std::find(customHandlers.begin(), customHandlers.end(), handler);
	if(findIt != customHandlers.end()) customHandlers.erase(findIt);
}

void LUNAEvents::DoUnsubscribeAllCustom()
{
	customHandlers.erase(customHandlers.begin(), customHandlers.end());
}

// Subscribe given handler to message
LuaFunction LUNAEvents::Subscribe(const std::string& message, const LuaFunction& handler)
{
	if(!handler)
	{
		LUNA_LOGE("Cannot subscribe handler. Invalid handler value");
		return nil;
	}

	RUN_DELAYED_ACTION(DoSubscribe(message, handler));
	return handler;
}

// Unsubsribe given handler from message
void LUNAEvents::Unsubscribe(const std::string& message, const LuaFunction& handler)
{
	RUN_DELAYED_ACTION(DoUnsubscribe(message, handler));
}

// Unsubscribe all messages for given message
void LUNAEvents::UnsubscribeAll(const std::string& message)
{
	RUN_DELAYED_ACTION(DoUnsubscribeAll(message));
}

// Subscribe custom handler calling for each message
LuaFunction LUNAEvents::SubscribeCustom(const LuaFunction& handler)
{
	if(!handler)
	{
		LUNA_LOGE("Cannot subscribe custom handler. Invalid handler value");
		return nil;
	}

	RUN_DELAYED_ACTION(DoSubscribeCustom(handler));
	return handler;
}

// Unsubscribe given custom handler
void LUNAEvents::UnsubscribeCustom(const LuaFunction& handler)
{
	RUN_DELAYED_ACTION(DoUnsubscribeCustom(handler));
}

// Unsubscrive all custom handlers
void LUNAEvents::UnsubscribeAllCustom()
{
	RUN_DELAYED_ACTION(DoUnsubscribeAllCustom());
}
