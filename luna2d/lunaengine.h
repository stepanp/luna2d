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

#pragma once

#include "lunaplatform.h"
#include "lunaconstants.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace luna2d{

class LuaScript;
class LUNAFiles;
class LUNALog;
class LUNAPlatformUtils;
class LUNAPrefs;
class LUNAModule;
class LUNAAssets;
class LUNAGraphics;
class LUNAScenes;
class LUNASizes;
class LUNADebug;
class LUNAConfig;

class LUNAEngine
{
private:
	LUNAEngine();
public:
	~LUNAEngine();

private:
	LUNAFiles* files;
	LUNALog* log;
	LUNAPlatformUtils* platformUtils;
	LUNAPrefs* prefs;

	LuaScript* lua;
	LUNAAssets* assets;
	LUNAGraphics* graphics;
	LUNAScenes* scenes;
	LUNASizes* sizes;
	LUNADebug* debug;

	std::unordered_map<std::string, LUNAModule*> modules;

	std::shared_ptr<LUNAConfig> config;
	bool initialized;

public:
	// Assemble engine with platform-specific modules. Must be called before "Initialize" method
	void Assemble(LUNAFiles* files, LUNALog* log, LUNAPlatformUtils* platformUtils, LUNAPrefs* prefs);
	void Initialize(int screenWidth, int screenHeight);
	void Deinitialize();
	std::shared_ptr<LUNAConfig> GetConfig();
	std::string GetGameName(); // Get name of runned game
	void LoadModules();
	void UnloadModules();
	void RunEmbeddedScripts();
	bool IsInitialized();

	void MainLoop();
	void OnTouchDown(float x, float y, int touchIndex);
	void OnTouchMoved(float x, float y, int touchIndex);
	void OnTouchUp(float x, float y, int touchIndex);

	// Get module by name
	template<typename Module>
	Module* GetModule(const std::string& name)
	{
		if(!modules.count(name)) return nullptr;
		return static_cast<Module*>(modules[name]);
	}

	template<typename Module>
	static Module* SharedModule(const std::string& name) { return Shared()->GetModule<Module>(name); }

	static LUNAEngine* Shared(); // Get shared instance of engine
	inline static LUNAFiles* SharedFiles() { return Shared()->files; }
	inline static LUNALog* SharedLog() { return Shared()->log; }
	inline static LUNAPlatformUtils* SharedPlatformUtils() { return Shared()->platformUtils; }
	inline static LUNAPrefs* SharedPrefs() { return Shared()->prefs; }
	inline static LuaScript* SharedLua() { return Shared()->lua; }
	inline static LUNAAssets* SharedAssets() { return Shared()->assets; }
	inline static LUNAGraphics* SharedGraphics() { return Shared()->graphics; }
	inline static LUNAScenes* SharedScenes() { return Shared()->scenes; }
	inline static LUNASizes* SharedSizes() { return Shared()->sizes; }
	inline static LUNADebug* SharedDebug() { return Shared()->debug; }
};

}
