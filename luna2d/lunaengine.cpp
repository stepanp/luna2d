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

#include "lunaengine.h"
#include "lunalua.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunaplatformutils.h"
#include "lunaassets.h"
#include "lunagraphics.h"
#include "lunaaudio.h"
#include "lunascenes.h"
#include "lunasizes.h"
#include "lunaevents.h"
#include "lunastrings.h"
#include "lunadebug.h"
#include "lunaconfig.h"
#include "lunamath.h"
#include "lunabindings.h"
#include "lunaservices.h"
#include "lunabindservices.h"

using namespace luna2d;

LUNAEngine::LUNAEngine()
{
}

LUNAEngine::~LUNAEngine()
{
	Deinitialize();
}

// Assemble engine with platform-specific modules. Must be called before "Iznitialize" method
void LUNAEngine::Assemble(LUNAFiles *files, LUNALog *log, LUNAPlatformUtils *platformUtils, LUNAPrefs* prefs, LUNAServices* services)
{
	this->files = files;
	this->log = log;
	this->platformUtils = platformUtils;
	this->prefs = prefs;
	this->services = services;
}

void LUNAEngine::Initialize(int screenWidth, int screenHeight)
{
	if(initialized) return;

	// Read config file
	config = std::make_shared<LUNAConfig>();
	if(!config->Read())
	{
		LUNA_LOGE("Error with reading config. Stop initializing");
		return;
	}

	lua = new LuaScript();

	// Make "luna" table in global table
	LuaTable tblGlobal = lua->GetGlobalTable();
	LuaTable tblLuna(lua);
	tblGlobal.SetField("luna", tblLuna);

	sizes = new LUNASizes(screenWidth, screenHeight, config);
	assets = new LUNAAssets();
	graphics = new LUNAGraphics();
	audio = new LUNAAudio();
	scenes = new LUNAScenes();
	events = new LUNAEvents();
	strings = new LUNAStrings();

#ifdef LUNA_DEBUG
	debug = new LUNADebug();
#endif

	RunEmbeddedScripts();
	DoBindings();

	services->LoadServices();
	BindServices();

	initialized = true;
}

void LUNAEngine::Run()
{
	if(!initialized) return;

	// Run main lua script
	if(!lua->DoFile("scripts/main.lua")) LUNA_RETURN_ERR("\"main.lua\" not found. Stop running");

	// Call "luna.main" function
	auto tblLuna = lua->GetGlobalTable().GetTable("luna");
	if(tblLuna)
	{
		auto fnMain = tblLuna.GetFunction("main");
		if(fnMain) fnMain.CallVoid();
	}
}

void LUNAEngine::Deinitialize()
{
	config.reset();

	delete services;
	delete assets;
	delete graphics;
	delete scenes;
	delete audio;
	delete sizes;
	delete strings;

#ifdef LUNA_DEBUG
	delete debug;
	debug = nullptr;
#endif

	delete events;
	delete lua;
	delete files;
	delete platformUtils;
	delete log;

	assets = nullptr;
	graphics = nullptr;
	scenes = nullptr;
	audio = nullptr;
	sizes = nullptr;
	events = nullptr;
	strings = nullptr;
	lua = nullptr;
	files = nullptr;
	platformUtils = nullptr;
	log = nullptr;
	services = nullptr;

	initialized = false;
}

std::shared_ptr<const LUNAConfig> LUNAEngine::GetConfig()
{
	return config;
}

 // Get name of runned game
std::string LUNAEngine::GetGameName()
{
	return config->gameName;
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

// Enable/disable handling OnPause/OnResume events
void LUNAEngine::EnablePauseHandling(bool enable)
{
	enablePauseHandling = enable;
}

void LUNAEngine::MainLoop()
{
	graphics->OnUpdate();
}

void LUNAEngine::OnPause()
{
	if(!enablePauseHandling) return;

	graphics->OnPause();
	audio->OnPause();
}

void LUNAEngine::OnResume()
{
	if(!enablePauseHandling) return;

	graphics->OnResume();
	audio->OnResume();
}

void LUNAEngine::OnTouchDown(float x, float y, int touchIndex)
{
	if(graphics->IsPaused()) return;

	glm::vec2 pos = graphics->GetCamera()->Unproject(glm::vec2(x, y));
	scenes->OnTouchDown(pos.x, pos.y, touchIndex);
}

void LUNAEngine::OnTouchMoved(float x, float y, int touchIndex)
{
	if(graphics->IsPaused()) return;

	glm::vec2 pos = graphics->GetCamera()->Unproject(glm::vec2(x, y));
	scenes->OnTouchMoved(pos.x, pos.y, touchIndex);
}

void LUNAEngine::OnTouchUp(float x, float y, int touchIndex)
{
	if(graphics->IsPaused()) return;

	glm::vec2 pos = graphics->GetCamera()->Unproject(glm::vec2(x, y));
	scenes->OnTouchUp(pos.x, pos.y, touchIndex);
}

// Get shared instance of engine
LUNAEngine* LUNAEngine::Shared()
{
	static LUNAEngine sharedEngine;
	return &sharedEngine;
}
