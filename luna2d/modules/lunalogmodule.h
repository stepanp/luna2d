//---------------------------------
// luna2d engine
// lunalogmodule.h - Log module
// Copyright 2014 Stepan Prokofjev
//---------------------------------

#pragma once

#include "lunamodule.h"

namespace luna2d{

class LUNALogModule : public LUNAModule
{
private:
	// Proxy for "__call" metametod
	// First param of "__call" metamethod is table
	// Because, we need use this proxy to filter this param
	void CallProxy(LuaTable, const char* message);

public:
	virtual void Load(LuaScript *lua);
};

}
