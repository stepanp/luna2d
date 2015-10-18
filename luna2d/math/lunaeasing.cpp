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

#include "lunaeasing.h"
#include "lunamath.h"

using namespace luna2d;
using namespace luna2d::easing;

float luna2d::easing::Linear(float t)
{
	return t;
}

float luna2d::easing::QuadraticIn(float t)
{
	return t * t;
}

float luna2d::easing::QuadraticOut(float t)
{
	return -(t * (t - 2));
}

float luna2d::easing::QuadraticInOut(float t)
{
	if(t < 0.5f) return 2 * t * t;
	return (-2 * t * t) + (4 * t) - 1;
}

float luna2d::easing::CubicIn(float t)
{
	return t * t * t;
}

float luna2d::easing::CubicOut(float t)
{
	float f = (t - 1);
	return f * f * f + 1;
}

float luna2d::easing::CubicInOut(float t)
{
	if(t < 0.5f) return 4 * t * t * t;
	else
	{
		float f = ((2 * t) - 2);
		return 0.5 * f * f * f + 1;
	}
}

float luna2d::easing::QuadricIn(float t)
{
	return t * t * t * t;
}

float luna2d::easing::QuadricOut(float t)
{
	float f = (t - 1);
	return f * f * f * (1 - t) + 1;
}

float luna2d::easing::QuadricInOut(float t)
{
	if(t < 0.5f) return 8 * t * t * t * t;
	else
	{
		float f = (t - 1);
		return -8 * f * f * f * f + 1;
	}
}

float luna2d::easing::QuinticIn(float t)
{
	return t * t * t * t * t;
}

float luna2d::easing::QuinticOut(float t)
{
	float f = (t - 1);
	return f * f * f * f * f + 1;
}

float luna2d::easing::QuinticInOut(float t)
{
	if(t < 0.5f) return 16 * t * t * t * t * t;
	else
	{
		float f = ((2 * t) - 2);
		return 0.5f * f * f * f * f * f + 1;
	}
}

float luna2d::easing::SineIn(float t)
{
	return std::sin((t - 1) * M_PI_2) + 1;
}


float luna2d::easing::SineOut(float t)
{
	return std::sin(t * M_PI_2);
}

float luna2d::easing::SineInOut(float t)
{
	return 0.5f * (1 - cos(t * M_PI));
}

float luna2d::easing::CircularIn(float t)
{
	return 1 - std::sqrt(1 - (t * t));
}

float luna2d::easing::CircularOut(float t)
{
	return std::sqrt((2 - t) * t);
}

float luna2d::easing::CircularInOut(float t)
{
	if(t < 0.5f) return 0.5f * (1 - std::sqrt(1 - 4 * (t * t)));
	return 0.5f * (std::sqrt(-((2 * t) - 3) * ((2 * t) - 1)) + 1);
}

float luna2d::easing::ExpIn(float t)
{
	return (t == 0.0f) ? t : std::pow(2, 10 * (t - 1));
}

float luna2d::easing::ExpOut(float t)
{
	return (t == 1.0) ? t : 1 - std::pow(2, -10 * t);
}

float luna2d::easing::ExpInOut(float t)
{
	if(t == 0.0f || t == 1.0f) return t;
	else if(t < 0.5f) return 0.5f * std::pow(2, (20 * t) - 10);
	else return -0.5f * std::pow(2, (-20 * t) + 10) + 1;
}

float luna2d::easing::ElasticIn(float t)
{
	return std::sin(13 * M_PI_2 * t) * std::pow(2, 10 * (t - 1));
}

float luna2d::easing::ElasticOut(float t)
{
	return std::sin(-13 * M_PI_2 * (t + 1)) * std::pow(2, -10 * t) + 1;
}

float luna2d::easing::ElasticInOut(float t)
{
	if(t < 0.5f) return 0.5f * std::sin(13 * M_PI_2 * (2 * t)) * std::pow(2, 10 * ((2 * t) - 1));
	return 0.5f * (std::sin(-13 * M_PI_2 * ((2 * t - 1) + 1)) * std::pow(2, -10 * (2 * t - 1)) + 2);
}

float luna2d::easing::BackIn(float t)
{
	return t * t * t - t * std::sin(t * M_PI);
}

float luna2d::easing::BackOut(float t)
{
	float f = (1 - t);
	return 1 - (f * f * f - f * std::sin(f * M_PI));
}

float luna2d::easing::BackInOut(float t)
{
	if(t < 0.5f)
	{
		float f = 2 * t;
		return 0.5 * (f * f * f - f * std::sin(f * M_PI));
	}
	else
	{
		float f = (1 - (2 * t - 1));
		return 0.5f * (1 - (f * f * f - f * std::sin(f * M_PI))) + 0.5f;
	}
}

float luna2d::easing::BounceIn(float t)
{
	return 1 - BounceOut(1 - t);
}

float luna2d::easing::BounceOut(float t)
{
	if(t < 4/11.0) return (121 * t * t)/16.0;
	else if(t < 8/11.0) return (363/40.0 * t * t) - (99/10.0 * t) + 17/5.0;
	else if(t < 9/10.0) return (4356/361.0 * t * t) - (35442/1805.0 * t) + 16061/1805.0;
	else return (54/5.0 * t * t) - (513/25.0 * t) + 268/25.0;
}

float luna2d::easing::BounceInOut(float t)
{
	if(t < 0.5) return 0.5f * BounceIn(t * 2);
	return 0.5f * BounceOut(t * 2 - 1) + 0.5;
}
