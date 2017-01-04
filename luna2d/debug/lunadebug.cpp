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

#ifdef LUNA_DEBUG

#include "lunadebug.h"
#include "lunalua.h"
#include "lunawatcher.h"
#include "lunaprofiler.h"

using namespace luna2d;

LUNADebug::LUNADebug()
{
	LuaScript* lua = LUNAEngine::SharedLua();

	// Initialize watcher
	watcher = std::make_shared<LUNAWatcher>();

	// Initialize profiler
	profiler = std::make_shared<LUNAProfiler>();

	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblDebug(lua);

	tblDebug.SetField("addToWatcher", LuaFunction(lua, watcher.get(), &LUNAWatcher::AddTable));
	tblDebug.SetField("removeFromWatcher", LuaFunction(lua, watcher.get(), &LUNAWatcher::RemoveTable));

	tblLuna.SetField("debug", tblDebug);
}

std::shared_ptr<LUNAWatcher> LUNADebug::GetWatcher()
{
	return watcher;
}

std::shared_ptr<luna2d::LUNAProfiler> LUNADebug::GetProfiler()
{
	return profiler;
}

#endif
