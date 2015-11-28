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

#include "lunaparticlesystem.h"

using namespace luna2d;

LUNAParticleSystem::LUNAParticleSystem(const LuaTable& luaParams)
{
	for(auto& entry : luaParams)
	{
		auto params = std::make_shared<LUNAParticleParams>(entry.second.ToTable());
		emitters.push_back(std::make_shared<LUNAParticleEmitter>(params));
	}
}

LUNAParticleSystem::LUNAParticleSystem(const std::vector<std::shared_ptr<LUNAParticleParams>>& params)
{
	for(auto& emitterParams : params)
	{
		emitters.push_back(std::make_shared<LUNAParticleEmitter>(emitterParams));
	}
}

float LUNAParticleSystem::GetX()
{
	return pos.x;
}

float LUNAParticleSystem::GetY()
{
	return pos.y;
}

void LUNAParticleSystem::SetX(float x)
{
	SetPos(x, pos.y);
}

void LUNAParticleSystem::SetY(float y)
{
	SetPos(pos.x, y);
}

glm::vec2 LUNAParticleSystem::GetPos()
{
	return pos;
}

void LUNAParticleSystem::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;

	for(auto& emitter : emitters) emitter->SetPos(pos);
}

bool LUNAParticleSystem::IsLoop()
{
	return loop;
}

void LUNAParticleSystem::SetLoop(bool loop)
{
	this->loop = loop;
}

bool LUNAParticleSystem::IsRunning()
{
	return running;
}

// Start or resume emitting
void LUNAParticleSystem::Start()
{
	for(auto& emitter : emitters)
	{
		if(!emitter->IsFinished())
		{
			emitter->Start();
			running = true;
		}
	}
}

// Stop emitting without reset duration
void LUNAParticleSystem::Pause()
{
	for(auto& emitter : emitters) emitter->Pause();
	running = false;
}

// Stop emitting
void LUNAParticleSystem::Stop()
{
	for(auto& emitter : emitters) emitter->Stop();
	running = false;
}

void LUNAParticleSystem::Update(float dt)
{
	bool allFinished = false;

	for(auto& emitter : emitters)
	{
		emitter->Update(dt);
		if(!emitter->IsFinished()) allFinished = true;
	}

	if(allFinished && running)
	{
		running = false;

		// Restart all emitters if loop is enabled
		if(loop)
		{
			Stop();
			Start();
		}
	}
}

void LUNAParticleSystem::Render()
{
	for(auto& emitter : emitters) emitter->Render();
}
