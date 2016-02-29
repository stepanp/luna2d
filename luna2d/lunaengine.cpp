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
#include "lunastrings.h"
#include "lunadebug.h"
#include "lunaconfig.h"
#include "lunamath.h"
#include "lunabindings.h"
#include "lunasdkapi.h"

using namespace luna2d;

LUNAEngine::LUNAEngine()
{
}

LUNAEngine::~LUNAEngine()
{
	Deinitialize();
}

// Assemble engine with platform-specific modules. Must be called before "Iznitialize" method
void LUNAEngine::Assemble(LUNAFiles *files, LUNALog *log, LUNAPlatformUtils *platformUtils, LUNAPrefs* prefs)
{
	this->files = files;
	this->log = log;
	this->platformUtils = platformUtils;
	this->prefs = prefs;
}

void LUNAEngine::SetSdkApi(LUNASdkApi* sdkApi)
{
	this->sdkApi = sdkApi;
}

void LUNAEngine::Initialize(int screenWidth, int screenHeight)
{
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

	sizes = new LUNASizes(screenWidth, screenHeight, config.get());
	assets = new LUNAAssets();
	graphics = new LUNAGraphics();
	audio = new LUNAAudio();
	scenes = new LUNAScenes();
	strings = new LUNAStrings();
	debug = new LUNADebug();

	math::InitializeRandom();
	RunEmbeddedScripts();
	DoBindings();

	if(sdkApi) sdkApi->LoadSdkModules();

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
	config.reset();

	delete sdkApi;
	delete assets;
	delete graphics;
	delete scenes;
	delete audio;
	delete sizes;
	delete strings;
	delete debug;
	delete lua;
	delete files;
	delete platformUtils;
	delete log;

	assets = nullptr;
	graphics = nullptr;
	scenes = nullptr;
	audio = nullptr;
	sizes = nullptr;
	strings = nullptr;
	debug = nullptr;
	lua = nullptr;
	files = nullptr;
	platformUtils = nullptr;
	log = nullptr;
	sdkApi = nullptr;

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

void LUNAEngine::MainLoop()
{
	graphics->OnUpdate();
}

void LUNAEngine::OnPause()
{
	graphics->OnPause();
	audio->OnPause();
}

void LUNAEngine::OnResume()
{
	graphics->OnResume();
	audio->OnResume();
}

void LUNAEngine::OnTouchDown(float x, float y, int touchIndex)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchDown(pos.x, pos.y, touchIndex);
}

void LUNAEngine::OnTouchMoved(float x, float y, int touchIndex)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchMoved(pos.x, pos.y, touchIndex);
}

void LUNAEngine::OnTouchUp(float x, float y, int touchIndex)
{
	glm::vec2 pos = sizes->ScreenToVirtual(glm::vec2(x, y));
	scenes->OnTouchUp(pos.x, pos.y, touchIndex);
}

// Get shared instance of engine
LUNAEngine* LUNAEngine::Shared()
{
	static LUNAEngine sharedEngine;
	return &sharedEngine;
}
