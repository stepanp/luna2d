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

#include "lunaengine.h"
#include "lunaeasing.h"
#include <cmath>

// On some compilers, M_PI constant isn't defined
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
	#define M_PI_2 1.57079632679489661923f
#endif


//-----------
// Math utils
//-----------
namespace luna2d{ namespace math{

const float DEGREES_TO_RADIANS = M_PI / 180.0f;
const float RADIANS_TO_DEGREES = 180.0f / M_PI;

void InitializeRandom();
float RandomFloat(float a, float b);  // Generate random float number in range [a,b]
float Avg(const std::vector<float>& values); // Calculate average value of given vector
float DegreesToRadians(float degrees); // Conver degrees to radians
float RadiansToDegrees(float radians); // Conver radians to degrees
int NearestPowerOfTwo(int value); // Get nearest power of two size to given value
bool IsPowerOfTwo(int value); // Check value for power of two
float Lerp(float a, float b, float t); // Interpolation between "a" and "b" by time "t". "t" must be in range[0,1]
float EaseLerp(float a, float b, float t, LUNAEasingFunc easing); // Interpolation between "a" and "b" using given easing. "t" must be in range[0,1]

}}
