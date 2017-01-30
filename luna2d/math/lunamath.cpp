//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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
#include "lunalog.h"
#include <random>

using namespace luna2d;
using namespace luna2d::math;

static std::default_random_engine RANDOM_GENERATOR;

// Generate random float number with uniform distribution in range [a,b]
float luna2d::math::RandomFloat(float a, float b)
{
	std::uniform_real_distribution<float> distribution(a, b);
	return distribution(RANDOM_GENERATOR);
}

// Generate random float number with normal distribution in range [a,b]
float luna2d::math::RandomFloatNormal(float a, float b)
{
	float dist = b - a;
	std::normal_distribution<float> distribution(a + dist / 2.0f, dist / 2.0f);
	return std::min(std::max(distribution(RANDOM_GENERATOR), a), b);
}

// Generate random float number with piecewise linear distribution
float luna2d::math::RandomFloatPiecewise(const std::vector<float>& intervals, const std::vector<float>& weights)
{
	if(intervals.empty() || intervals.size() != weights.size())
	{
		LUNA_LOGE("Incorrect intervals or weights for piecewise linear random distribution");
		return 0;
	}

	std::piecewise_linear_distribution<float> distribution(intervals.begin(), intervals.end(), weights.begin());
	return distribution(RANDOM_GENERATOR);
}

// Generate random integer number with uniform distribution in range [a,b]
int luna2d::math::RandomInt(int a, int b)
{
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(RANDOM_GENERATOR);
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

// Interpolation between "a" and "b" by time "t". "t" must be in range [0,1]
float luna2d::math::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// Interpolation between "a" and "b" using given easing. "t" must be in range [0,1]
float luna2d::math::EaseLerp(float a, float b, float t, const std::function<float(float)>& easing)
{
	return a + (b - a) * easing(t);
}

// Get midpoint between two given vectors
glm::vec2 luna2d::math::Midpoint(const glm::vec2& vec1, const glm::vec2& vec2)
{
	return (vec1 + vec2) * 0.5f;
}

// Get angle between given vectors (in degrees)
float luna2d::math::AngleBetween(const glm::vec2& vec1, const glm::vec2& vec2)
{
	return glm::degrees(AngleBetweenr(vec1, vec2));
}

// Get angle between given vectors (in radians)
float luna2d::math::AngleBetweenr(const glm::vec2& vec1, const glm::vec2& vec2)
{
	glm::vec2 angleVec = vec1 - vec2;

	if(glm::all(glm::lessThan(glm::abs(angleVec), glm::vec2(GLM_EPSILON))))
		return 0.0f;
	else
		return glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(angleVec));
}
