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

#include "lunaparticleemitter.h"
#include "lunaassets.h"
#include "lunamath.h"

using namespace luna2d;

LUNAParticleEmitter::LUNAParticleEmitter(const std::shared_ptr<LUNAParticleParams>& params) :
	params(params),
	emitTime(params->initSpawnDelay)
{
	LUNAAssets* assets = LUNAEngine::SharedAssets();

	for(const auto& path : params->textures)
	{
		auto region = assets->GetAssetByPath<LUNATextureRegion>(path);
		if(!region.expired())
		{
			sourceSprites.push_back(std::make_shared<LUNASprite>(region));
			continue;
		}

		auto texture = assets->GetAssetByPath<LUNATexture>(path);
		if(!texture.expired())
		{
			sourceSprites.push_back(std::make_shared<LUNASprite>(texture));
			continue;
		}

		LUNA_LOGE("Invalid texture or texture region \"%s\"", path.c_str());
	}

	if(params->textureSelectionMode == LUNATextureSelectionMode::SHUFFLE && !sourceSprites.empty())
	{
		std::random_shuffle(sourceSprites.begin(), sourceSprites.end());
	}
}

std::shared_ptr<LUNASprite> LUNAParticleEmitter::GetNextSprite()
{
	switch(params->textureSelectionMode)
	{
	case LUNATextureSelectionMode::SERIAL:
	case LUNATextureSelectionMode::SHUFFLE:
		{
			auto ret = sourceSprites[spriteIndex];

			spriteIndex++;
			if(spriteIndex >= sourceSprites.size()) spriteIndex = 0;

			return ret;
		}
	case LUNATextureSelectionMode::RANDOM:
		return sourceSprites[math::RandomInt(0, sourceSprites.size() - 1)];
	}

	return sourceSprites[0];
}

glm::vec2 LUNAParticleEmitter::GetSpawnPos()
{
	switch(params->spawnAreaMode)
	{
	case LUNASpawnAreaMode::POINT:
		return pos;
	case LUNASpawnAreaMode::RECT:
		{
			LUNARect& rect = params->spawnRect;
			glm::vec2 ret;

			ret.x = pos.x + rect.x + math::RandomFloat(0.0f, rect.width);
			ret.y = pos.y + rect.y + math::RandomFloat(0.0f, rect.height);

			return ret;
		}
	case LUNASpawnAreaMode::CIRCLE:
		float r = math::RandomFloat(0, params->spawnCircleR);
		float angle = math::RandomFloat(0.0f, 360.0f);
		glm::vec2 radius = glm::rotate(glm::vec2(r, 0), glm::radians(angle));

		return radius + pos;
	}

	return pos;
}

void LUNAParticleEmitter::Emit()
{
	for(int i = 0; i < params->spawnCount; i++)
	{
		auto sprite = GetNextSprite();
		auto spawnPos = GetSpawnPos();
		auto particle = std::make_shared<LUNAParticle>(sprite, params);

		particle->SetPos(spawnPos.x, spawnPos.y);
		if(params->dirFromEmitter && params->spawnAreaMode != LUNASpawnAreaMode::POINT)
		{
			glm::vec2 relativePos = spawnPos - pos;
			float angle = glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(relativePos));

			particle->SetDirection(glm::degrees(angle));
		}

		particles.push_back(particle);
	}
}

void LUNAParticleEmitter::UpdateDuration(float dt)
{
	if(params->duration == 0 || !running) return;

	durationTime += dt;
	if(durationTime >= params->duration) running = false;
}

void LUNAParticleEmitter::UpdateEmit(float dt)
{
	if(sourceSprites.empty() || !running) return;

	emitTime -= dt;

	if(emitTime <= 0)
	{
		Emit();
		emitTime = params->spawnDelay;
	}
}

void LUNAParticleEmitter::UpdateParticles(float dt)
{
	size_t count = particles.size();
	for(size_t i = 0; i < count; i++)
	{
		particles[i]->Update(dt);
		if(particles[i]->IsDeleted())
		{
			particles.erase(particles.begin() + i);
			count = particles.size();
			i--;
		}
	}
}

bool LUNAParticleEmitter::IsFinished()
{
	return params->duration > 0 && durationTime >= params->duration;
}

glm::vec2 LUNAParticleEmitter::GetPos()
{
	return pos;
}

void LUNAParticleEmitter::SetPos(const glm::vec2& pos)
{
	glm::vec2 newPos = pos + params->emitterPos;

	if(params->attached)
	{
		for(auto& particle : particles)
		{
			glm::vec2 relativePos = particle->GetPos() - this->pos;
			particle->SetPos(relativePos.x + newPos.x, relativePos.y + newPos.y);
		}
	}

	this->pos = newPos;
}

bool LUNAParticleEmitter::IsRunning()
{
	return running;
}

// Start or resume emitting
void LUNAParticleEmitter::Start()
{
	running = true;
}

// Stop emitting without reset duration
void LUNAParticleEmitter::Pause()
{
	running = false;
}

// Stop emitting
void LUNAParticleEmitter::Stop()
{
	running = false;
	durationTime = 0;
	emitTime = 0;
}

void LUNAParticleEmitter::Update(float dt)
{
	UpdateDuration(dt);
	UpdateEmit(dt);
	UpdateParticles(dt);
}

void LUNAParticleEmitter::Render()
{
	for(auto& particle : particles) particle->Render();
}
