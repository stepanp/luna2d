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

#include "lunaparticle.h"
#include "lunamath.h"
#include "lunaeasing.h"

using namespace luna2d;

LUNAParticle::LUNAParticle(const std::shared_ptr<LUNASprite>& sprite, const std::shared_ptr<LUNAParticleParams>& params) :
	LUNASprite(*sprite)
{
	lifetime = math::RandomFloat(params->lifetimeMin, params->lifetimeMax);
	lifetimeTotal = lifetime;

	alphaBegin = math::RandomFloat(params->alphaBeginMin, params->alphaBeginMax);
	alphaEnd = math::RandomFloat(params->alphaEndMin, params->alphaEndMax);
	speed = math::RandomFloat(params->speedMin, params->speedMax);

	float dirAngle = math::RandomFloat(params->directionMin, params->directionMax);
	dir = glm::rotate(glm::vec2(1.0f, 0), glm::radians(dirAngle));
}

bool LUNAParticle::IsDeleted()
{
	return deleted;
}

void LUNAParticle::Update(float dt)
{
	if(deleted) return;

	lifetime -= dt;
	if(lifetime <= 0)
	{
		deleted = true;
		lifetime = 0;
	}

	float percent = 1.0f - (lifetime / lifetimeTotal);

	SetAlpha(math::EaseLerp(alphaBegin, alphaEnd, percent, easing::Linear));

	glm::vec2 newPos = GetPos() + (dir * (speed * dt));
	SetPos(newPos.x, newPos.y);
}
