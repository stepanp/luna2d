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

#include "lunaparticleparams.h"
#include "lunavector2.h"

using namespace luna2d;

LUNAParticleParams::LUNAParticleParams(const LuaTable& luaParams)
{
	if(luaParams.HasField("spawnAreaMode"))
	{
		std::string spawnAreaModeStr = luaParams.GetString("spawnAreaMode");
		if(SPAWN_AREA_MODE.HasKey(spawnAreaModeStr))
		{
			spawnAreaMode = SPAWN_AREA_MODE.FromString(spawnAreaModeStr);
			if(spawnAreaMode == LUNASpawnAreaMode::RECT) spawnRect = luaParams.GetField<LUNARect>("spawnRect");
			else if(spawnAreaMode == LUNASpawnAreaMode::CIRCLE) spawnCircleR = luaParams.GetField<float>("spawnCircleR");
		}
		else LUNA_LOGE("Unsupported spawn area mode: \"%s\"", spawnAreaModeStr.c_str());
	}

	if(luaParams.HasField("textureSelectionMode"))
	{
		std::string textureSelectionModeStr = luaParams.GetString("textureSelectionMode");
		if(TEXTURE_SELECTION_MODE.HasKey(textureSelectionModeStr))
		{
			textureSelectionMode = TEXTURE_SELECTION_MODE.FromString(textureSelectionModeStr);
		}
		else LUNA_LOGE("Unsupported texture selection mode: \"%s\"", textureSelectionModeStr.c_str());
	}

	textures = luaParams.GetField<std::vector<std::string>>("textures");
	if(textures.empty()) LUNA_LOGE("Particle emitter must have at least one texture or texture region");

	emitterPos = luaParams.GetField<glm::vec2>("emitterPos");
	attached = luaParams.GetBool("attached");
	duration = luaParams.GetFloat("duration");
	spawnCount = luaParams.GetInt("spawnCount");
	spawnDelay = luaParams.GetFloat("spawnDelay");
	lifetimeMin = luaParams.GetFloat("lifetimeMin");
	lifetimeMax = luaParams.GetFloat("lifetimeMax");
	directionMin = luaParams.GetFloat("directionMin");
	directionMax = luaParams.GetFloat("directionMax");
	speedMin = luaParams.GetFloat("speedMin");
	speedMax = luaParams.GetFloat("speedMax");
	initAngleMin = luaParams.GetFloat("initAngleMin");
	initAngleMax = luaParams.GetFloat("initAngleMax");
	rotateMin = luaParams.GetFloat("rotateMin");
	rotateMax = luaParams.GetFloat("rotateMax");
	scaleBeginMin = luaParams.GetFloat("scaleBeginMin");
	scaleBeginMax = luaParams.GetFloat("scaleBeginMax");
	scaleEndMin = luaParams.GetFloat("scaleEndMin");
	scaleEndMax = luaParams.GetFloat("scaleEndMax");
	alphaBeginMin = luaParams.GetFloat("alphaBeginMin");
	alphaBeginMax = luaParams.GetFloat("alphaBeginMax");
	alphaEndMin = luaParams.GetFloat("alphaEndMin");
	alphaEndMax = luaParams.GetFloat("alphaEndMax");
}
