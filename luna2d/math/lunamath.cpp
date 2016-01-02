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

#include "lunamath.h"
#include <stdlib.h>
#include <time.h>

using namespace luna2d;
using namespace luna2d::math;

void luna2d::math::InitializeRandom()
{
	srand((unsigned int)time(0));
}

// Generate random float number in range [a,b]
float luna2d::math::RandomFloat(float a, float b)
{
	float percent = (float)(rand() % 101) / 100;
	return a + (b - a) * percent;
}

// Generate random integer in range [a,b]
int luna2d::math::RandomInt(int a, int b)
{
	return a + rand() % ((b + 1) - a);
}

// Calculate average value of given vector
float luna2d::math::Avg(const std::vector<float>& values)
{
	float sum = 0;
	for(float value : values) sum += value;
	return sum / values.size();
}

// Convert degrees to radians
float luna2d::math::DegreesToRadians(float degrees)
{
	return degrees * DEGREES_TO_RADIANS;
}

// Convert radians to degrees
float luna2d::math::RadiansToDegrees(float radians)
{
	return radians * RADIANS_TO_DEGREES;
}

// Get nearest power of two size to given value
int luna2d::math::NearestPowerOfTwo(int value)
{
	return std::pow(2, std::ceil(std::log(value) / std::log(2)));
}

// Check value for power of two
bool luna2d::math::IsPowerOfTwo(int value)
{
	return (value != 0) && ((value & (value - 1)) == 0);
}

// Interpolation between "a" and "b" by time "t". "t" must be in range[0,1]
float luna2d::math::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// Interpolation between "a" and "b" using given easing. "t" must be in range[0,1]
float luna2d::math::EaseLerp(float a, float b, float t, const std::function<float(float)>& easing)
{
	return a + (b - a) * easing(t);
}
