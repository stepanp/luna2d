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

	// To support any actions(e.g. subscribe/unsubscribe) with subsystem during run event handlers,
	// actions runs after processing events during sending or runs immediately otherwise
	// SEE: "ProcessInternalActions" and "RunInternalAction"
	std::vector<std::function<void()>> internalActions;
	bool processing = false;

private:
	// Send message from Lua
	// For maximize perfomance implemented as "lua_CFunction"
	static int LuaSend(lua_State* luaVm);

	void ProcessInternalActions();

	void RunInternalAction(const std::function<void()>& action);

public:
	// Subscribe given handler to message
	void Subscribe(const std::string& message, const LuaFunction& handler);

	// Unsubscribe all messages for given message
	void UnsubscribeAll(const std::string& message);

	// Send message with given params
	template<typename ... Args>
	void Send(const std::string& message, Args ... args)
	{
		auto itHandlers = handlersMap.find(message);
		if(itHandlers == handlersMap.end()) return;

		// Events is currently processing
		bool nestedSend = processing;
		processing = true;

		const auto& handlers = itHandlers->second;
		for(const auto& handler : handlers)
		{
			handler.CallVoid<Args ...>(args ...);
		}

		// If it's root send, finish events processing
		if(!nestedSend)
		{
			processing = false;
			ProcessInternalActions();
		}
	}
};

}
