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

#include "lunalua.h"
#include "lunagraphics.h"
#include "lunarect.h"
#include "utils/lunastringenum.h"

namespace luna2d{

enum class LUNATextureSelectionMode
{
	SERIAL,
	RANDOM,
	SHUFFLE,
};

const LUNAStringEnum<LUNATextureSelectionMode> TEXTURE_SELECTION_MODE =
{
	"serial",
	"random",
	"shuffle",
};


enum class LUNASpawnAreaMode
{
	POINT,
	RECT,
	CIRCLE,
};

const LUNAStringEnum<LUNASpawnAreaMode> SPAWN_AREA_MODE =
{
	"point",
	"rect",
	"circle",
};


class LUNAParticleParams
{
public:
	LUNAParticleParams(const LuaTable& luaParams);

public:
	// Mode of area where spawns particles
	LUNASpawnAreaMode spawnAreaMode = LUNASpawnAreaMode::POINT;

	// Rectangle for "LUNASpawnAreaMode::RECT" spawn area mode
	// "x" and "y" of this rect are offset from emiiter position
	LUNARect spawnRect;

	// Radius for "LUNASpawnAreaMode::CIRCLE" spawn area mode
	float spawnRectR = 0.0f;

	// List of textures or texture regions using to create particles
	// Stored as paths to asset in asset manager. Asset object gets from asset manager when emmiter created
	std::vector<std::string> textures;

	// Mode of selection texture from textures list for each new particle
	LUNATextureSelectionMode textureSelectionMode = LUNATextureSelectionMode::SHUFFLE;

	// Move particles with emmitter or not
	bool attached = false;

	// Duration of emitter working. "0" is infinite
	float duration = 0.0f;

	// Count of particles spawns at once
	int spawnCount = 1;

	// Delay between spawning
	float spawnDelay = 0.0f;

	// Lifetime range of particle
	float lifetimeMin = 0.0f;
	float lifetimeMax = 0.0f;

	// Particle direction range
	float dirMin = 0.0f;
	float dirMax = 0.0f;

	// Particle speed range
	float speedMin = 1.0f;
	float speedMax = 1.0f;

	// Initial rotation angle of particle range
	float initAngleMin = 0.0f;
	float initAngleMax = 0.0f;

	// Particle rotation angle per second range
	float rotateMin = 0.0f;
	float rotateMax = 0.0f;

	// Particle scale range on begin lifetime
	float scaleBeginMin = 0.0f;
	float scaleBeginMax = 0.0f;

	// Particle scale range on end lifetime
	float scaleEndMin = 0.0f;
	float scaleEndMax = 0.0f;

	// Particle alpha range on begin lifetime
	float alphaBeginMin = 1.0f;
	float alphaBeginMax = 1.0f;

	// Particle alpha range on end lifetime
	float alphaEndMin = 1.0f;
	float alphaEndMax = 1.0f;
};

}
