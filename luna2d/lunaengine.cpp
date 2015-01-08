//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaengine.h"
#include "lunalua.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunaplatformutils.h"
#include "lunaassets.h"
#include "lunagraphics.h"
#include "lunascenes.h"
#include "lunasizes.h"
#include "lunaconfig.h"
#include "lunamathutils.h"

#include "modules/lunamoduleslist.h"

using namespace luna2d;

LUNAEngine::LUNAEngine() : modules(modulesList)
{
	config = nullptr;

	lua = nullptr;
	files = nullptr;
	log = nullptr;
	platformUtils = nullptr;

	assets = nullptr;
	graphics = nullptr;
	scenes = nullptr;
	sizes = nullptr;

	initialized = false;
}

LUNAEngine::~LUNAEngine()
{
	Deinitialize();
}

// Assemble engine with platform-specific modules. Must be called before "Iznitialize" method
void LUNAEngine::Assemble(LUNAFiles *files, LUNALog *log, LUNAPlatformUtils *platformUtils)
{
	this->files = files;
	this->log = log;
	this->platformUtils = platformUtils;
}

void LUNAEngine::Initialize(int screenWidth, int screenHeight)
{
	// Read config file
	config = std::unique_ptr<LUNAConfig>(new LUNAConfig());
	config->Read();

	lua = new LuaScript();

	// Make "luna" table in global table
	LuaTable tblGlobal = lua->GetGlobalTable();
	LuaTable tblLuna(lua);
	tblGlobal.SetField("luna", tblLuna);

	LUNAMathUtils::InitializeRandom();
	RunEmbeddedScripts();
	LoadModules();

	sizes = new LUNASizes(screenWidth, screenHeight, config.get());
	assets = new LUNAAssets();
	graphics = new LUNAGraphics();
	scenes = new LUNAScenes();

	// Run main lua script
	if(!lua->DoFile("scripts/main.lua"))
	{
		LUNA_LOGE("\"main.lua\" not found. Stop initializing");
		return;
	}

	// Call "luna.main" function
	LuaFunction fnMain = tblLuna.GetFunction("main");
	if(fnMain != nil) fnMain.CallVoid();

	initialized = true;
}

void LUNAEngine::Deinitialize()
{
	UnloadModules();

	delete files;
	delete log;
	delete platformUtils;
	delete assets;
	delete graphics;
	delete scenes;
	delete sizes;
	delete lua;

	config = nullptr;
	files = nullptr;
	log = nullptr;
	platformUtils = nullptr;
	assets = nullptr;
	graphics = nullptr;
	scenes = nullptr;
	sizes = nullptr;
	lua = nullptr;

	initialized = false;
}

LUNAConfig* LUNAEngine::GetConfig()
{
	return config.get();
}

void LUNAEngine::LoadModules()
{
	// Load all modules
	for(auto& module : modules) module.second->Load(lua);
}

void LUNAEngine::UnloadModules()
{
	for(auto& module : modules) module.second->Unload();
}

void LUNAEngine::RunEmbeddedScripts()
{
	lua->DoString(LUNA_LUA_OOP_SUPPORT);
	lua->DoString(LUNA_LUA_USERDATA_PAIRS);
}

bool LUNAEngine::IsInitialized()
{
	return initialized;
}

void LUNAEngine::ReloadAssets()
{
	assets->ReloadAssets();
	graphics->GetRenderer()->ReloadDefaultShader();
}

void LUNAEngine::MainLoop()
{
	graphics->OnUpdate();
}

void LUNAEngine::OnTouchDown(float x, float y)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchDown(pos.x, pos.y);
}

void LUNAEngine::OnTouchMoved(float x, float y)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchMoved(pos.x, pos.y);
}

void LUNAEngine::OnTouchUp(float x, float y)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchUp(pos.x, pos.y);
}

// Get shared instance of engine
LUNAEngine* LUNAEngine::Shared()
{
	static LUNAEngine sharedEngine;
	return &sharedEngine;
}
