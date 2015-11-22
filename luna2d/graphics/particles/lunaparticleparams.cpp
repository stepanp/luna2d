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

static LUNAEasingFunc GetEasing(const LuaTable& luaParams, const std::string& name)
{
	auto it = EASINGS_MAP.find(luaParams.GetString(name));
	if(it != EASINGS_MAP.end()) return it->second;
	return easing::Linear;
}

LUNAParticleParams::LUNAParticleParams(const LuaTable& luaParams)
{
	if(luaParams == nil) LUNA_RETURN_ERR("Particle params must be a table");

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

	if(luaParams.HasField("gravityMotionMode"))
	{
		std::string gravityMotionModeStr = luaParams.GetString("gravityMotionMode");
		if(MOTION_MODE.HasKey(gravityMotionModeStr)) gravityMotionMode = MOTION_MODE.FromString(gravityMotionModeStr);
		else LUNA_LOGE("Unsupported gravity motion mode: \"%s\"", gravityMotionModeStr.c_str());
	}

	if(luaParams.HasField("speedMotionMode"))
	{
		std::string speedMotionModeStr = luaParams.GetString("speedMotionMode");
		if(MOTION_MODE.HasKey(speedMotionModeStr)) speedMotionMode = MOTION_MODE.FromString(speedMotionModeStr);
		else LUNA_LOGE("Unsupported speed motion mode: \"%s\"", speedMotionModeStr.c_str());
	}

	emitterPos = luaParams.GetField<glm::vec2>("emitterPos");
	attached = luaParams.GetBool("attached");
	duration = luaParams.GetFloat("duration");
	maxCount = luaParams.GetInt("maxCount");
	spawnCount = luaParams.GetField<LUNARangeInt>("spawnCount");
	spawnDelay = luaParams.GetFloat("spawnDelay");
	initSpawnDelay = luaParams.GetFloat("initSpawnDelay");
	lifetime = luaParams.GetField<LUNARangeFloat>("lifetime");
	direction = luaParams.GetField<LUNARangeFloat>("direction");
	gravity = luaParams.GetField<glm::vec2>("gravity");
	dirFromEmitter = luaParams.GetBool("dirFromEmitter");
	speedBegin = luaParams.GetField<LUNARangeFloat>("speedBegin");
	speedEnd = luaParams.GetField<LUNARangeFloat>("speedEnd");
	initAngle = luaParams.GetField<LUNARangeFloat>("initAngle");
	rotateBegin = luaParams.GetField<LUNARangeFloat>("rotateBegin");
	rotateEnd = luaParams.GetField<LUNARangeFloat>("rotateEnd");
	scaleBegin = luaParams.GetField<LUNARangeFloat>("scaleBegin");
	scaleEnd = luaParams.GetField<LUNARangeFloat>("scaleEnd");
	alphaBegin = luaParams.GetField<LUNARangeFloat>("alphaBegin");
	alphaEnd = luaParams.GetField<LUNARangeFloat>("alphaEnd");
	colorBegin = luaParams.GetField<LUNAColor>("colorBegin");
	colorEnd = luaParams.GetField<LUNAColor>("colorEnd");

	speedEasing = GetEasing(luaParams, "speedEasing");
	rotateEasing = GetEasing(luaParams, "rotateEasing");
	scaleEasing = GetEasing(luaParams, "scaleEasing");
	alphaEasing = GetEasing(luaParams, "alphaEasing");
	colorEasing = GetEasing(luaParams, "colorEasing");
}
