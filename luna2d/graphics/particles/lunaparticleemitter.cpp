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
	params(params)
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

				ret.x = pos.x + math::RandomFloat(rect.x, rect.width);
				ret.y = pos.y + math::RandomFloat(rect.y, rect.height);

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
		auto particle = std::make_shared<LUNAParticle>(sourceSprites[0], GetSpawnPos(), params);
		particles.push_back(particle);
	}
}

void LUNAParticleEmitter::UpdateEmit(float dt)
{
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

glm::vec2 LUNAParticleEmitter::GetPos()
{
	return pos;
}

void LUNAParticleEmitter::SetPos(const glm::vec2& pos)
{
	this->pos = pos + params->emitterPos;
}

void LUNAParticleEmitter::Update(float dt)
{
	UpdateEmit(dt);
	UpdateParticles(dt);
}

void LUNAParticleEmitter::Render()
{
	for(auto& particle : particles) particle->Render();
}
