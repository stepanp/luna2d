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

LUNAParticleValue::LUNAParticleValue(const LUNARangeFloat& begin, const LUNARangeFloat& end, LUNAEasingFunc easing) :
	begin(math::RandomFloat(begin.min, begin.max)),
	end(math::RandomFloat(end.min, end.max)),
	easing(easing)
{
}

LUNAParticleValue::LUNAParticleValue(float begin, float end, LUNAEasingFunc easing) :
	begin(begin),
	end(end),
	easing(easing)
{
}

float LUNAParticleValue::GetValue(float percent)
{
	return math::EaseLerp(begin, end, percent, easing);
}


LUNAParticle::LUNAParticle(const std::shared_ptr<LUNASprite>& sprite, const std::shared_ptr<LUNAParticleParams>& params) :
	LUNASprite(*sprite),
	lifetime(math::RandomFloat(params->lifetime.min, params->lifetime.max)),
	lifetimeTotal(lifetime),
	speed(params->speedBegin, params->speedEnd),
	rotate(params->rotateBegin, params->rotateEnd),
	alpha(params->alphaBegin, params->alphaEnd),
	scale(params->scaleBegin, params->scaleEnd),
	colorBegin(params->colorBegin),
	colorEnd(params->colorEnd)
{
	SetOriginToCenter();
	SetAngle(math::RandomFloat(params->initAngle.min, params->initAngle.max));
	SetDirection(math::RandomFloat(params->direction.min, params->direction.max));
}

bool LUNAParticle::IsDeleted()
{
	return deleted;
}

void LUNAParticle::SetColor(float r, float g, float b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void LUNAParticle::SetDirection(float angle)
{
	dir = glm::rotate(glm::vec2(1.0f, 0), glm::radians(angle));
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

	SetAlpha(alpha.GetValue(percent));
	SetScale(scale.GetValue(percent));
	SetAngle(GetAngle() + (rotate.GetValue(percent) * dt));

	float r = math::EaseLerp(colorBegin.r, colorEnd.r, percent, easing::Linear);
	float g = math::EaseLerp(colorBegin.g, colorEnd.g, percent, easing::Linear);
	float b = math::EaseLerp(colorBegin.b, colorEnd.b, percent, easing::Linear);
	SetColor(r, g, b);

	glm::vec2 newPos = GetPos() + (dir * (speed.GetValue(percent) * dt));
	SetPos(newPos.x, newPos.y);
}
