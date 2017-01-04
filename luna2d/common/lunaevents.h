//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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

namespace luna2d{

//----------------------------------------------------------
// Events subsystem
// Sending events implemented via lua stack
// It's allow sending messages lua <-> lua and lua <-> c++
// But is not optimal for using events as bridge c++ <-> c++
//----------------------------------------------------------
class LUNAEvents
{
public:
	LUNAEvents();

private:
	std::unordered_map<std::string, std::vector<LuaFunction>> handlersMap;
	std::vector<LuaFunction> customHandlers;

	// Some actions(Subscribe/unsubscribe/etc.) cannot be run during processing of event handlers
	// So, actions invoked during processing event handlers stores in "delayedActions" list
	// and runs after event handlers being processed.
	// SEE: "ProcessDelayedActions" and "RUN_DELAYED_ACTION" macro in "lunaevents.cpp"
	std::vector<std::function<void()>> delayedActions;
	bool processing = false;

private:
	// Send message from Lua
	// For maximize perfomance implemented as "lua_CFunction"
	static int LuaSend(lua_State* luaVm);

	void ProcessDelayedActions();

	void DoSubscribe(const std::string& message, const LuaFunction& handler);
	void DoUnsubscribe(const std::string& message, const LuaFunction& handler);
	void DoUnsubscribeAll(const std::string& message);
	void DoSubscribeCustom(const LuaFunction& handler);
	void DoUnsubscribeCustom(const LuaFunction& handler);
	void DoUnsubscribeAllCustom();

public:
	// Subscribe given handler to message
	LuaFunction Subscribe(const std::string& message, const LuaFunction& handler);

	// Unsubsribe given handler from message
	void Unsubscribe(const std::string& message, const LuaFunction& handler);

	// Unsubscribe all messages for given message
	void UnsubscribeAll(const std::string& message);

	// Subscribe custom handler calling for each message
	LuaFunction SubscribeCustom(const LuaFunction& handler);

	// Unsubscribe given custom handler
	void UnsubscribeCustom(const LuaFunction& handler);

	// Unsubscrive all custom handlers
	void UnsubscribeAllCustom();

	// Send message with given params
	template<typename ... Args>
	void Send(const std::string& message, const Args& ... args)
	{
		// Events is currently processing
		bool nestedSend = processing;
		processing = true;

		// Run custom handlers (runs for each message)
		for(const auto& customHandler : customHandlers)
		{
			customHandler.CallVoid<const std::string&, const Args& ...>(message, args ...);
		}

		// Run handlers for given message if exists
		auto itHandlers = handlersMap.find(message);
		if(itHandlers == handlersMap.end())
		{
			for(const auto& handler : itHandlers->second)
			{
				handler.CallVoid<const Args& ...>(args ...);
			}
		}

		// If it's root send, finish events processing
		if(!nestedSend)
		{
			processing = false;
			ProcessDelayedActions();
		}
	}
};

}
