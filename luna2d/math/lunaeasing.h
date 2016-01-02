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

#pragma once

#include "lunaengine.h"

namespace luna2d{

//----------------------------
// Common type of easing funcs
//----------------------------
typedef float (*LUNAEasingFunc)(float);

//-------------------------
// Easing functions library
//-------------------------
namespace easing{

float Linear(float t);
float QuadraticIn(float t);
float QuadraticOut(float t);
float QuadraticInOut(float t);
float CubicIn(float t);
float CubicOut(float t);
float CubicInOut(float t);
float QuadricIn(float t);
float QuadricOut(float t);
float QuadricInOut(float t);
float QuinticIn(float t);
float QuinticOut(float t);
float QuinticInOut(float t);
float SineIn(float t);
float SineOut(float t);
float SineInOut(float t);
float CircularIn(float t);
float CircularOut(float t);
float CircularInOut(float t);
float ExpIn(float t);
float ExpOut(float t);
float ExpInOut(float t);
float ElasticIn(float t);
float ElasticOut(float t);
float ElasticInOut(float t);
float BackIn(float t);
float BackOut(float t);
float BackInOut(float t);
float BounceIn(float t);
float BounceOut(float t);
float BounceInOut(float t);

}

//-------------------------------------------
// Map for access to easing functions by name
//-------------------------------------------
const std::unordered_map<std::string, LUNAEasingFunc> EASINGS_MAP =
{
	{ "linear", &easing::Linear },
	{ "quadraticIn", &easing::QuadraticIn },
	{ "quadraticOut", &easing::QuadraticOut },
	{ "quadraticInOut", &easing::QuadraticInOut },
	{ "cubicIn", &easing::CubicIn },
	{ "cubicOut", &easing::CubicOut },
	{ "cubicInOut", &easing::CubicInOut },
	{ "quadricIn", &easing::QuadricIn },
	{ "quadricOut", &easing::QuadricOut },
	{ "quadricInOut", &easing::QuadricInOut },
	{ "quinticIn", &easing::QuinticIn },
	{ "quinticOut", &easing::QuinticOut },
	{ "quinticInOut", &easing::QuinticInOut },
	{ "sineIn", &easing::SineIn },
	{ "sineOut", &easing::SineOut },
	{ "sineInOut", &easing::SineInOut },
	{ "circularIn", &easing::CircularIn },
	{ "circularOut", &easing::CircularOut },
	{ "circularInOut", &easing::CircularInOut },
	{ "expIn", &easing::ExpIn },
	{ "expOut", &easing::ExpOut },
	{ "expInOut", &easing::ExpInOut },
	{ "elasticIn", &easing::ElasticIn },
	{ "elasticOut", &easing::ElasticOut },
	{ "elasticInOut", &easing::ElasticInOut },
	{ "backIn", &easing::BackIn },
	{ "backOut", &easing::BackOut },
	{ "backInOut", &easing::BackInOut },
	{ "bounceIn", &easing::BounceIn },
	{ "bounceOut", &easing::BounceOut },
	{ "bounceInOut", &easing::BounceInOut },
};

}
