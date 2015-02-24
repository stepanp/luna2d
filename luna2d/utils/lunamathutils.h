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

#include <vector>
#include <cmath>

// On some compilers, M_PI constant isn't defined
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

namespace luna2d{

const float DEGREES_TO_RADIANS = M_PI / 180.0f;
const float RADIANS_TO_DEGREES = 180.0f / M_PI;

class LUNAMathUtils
{
private:
	LUNAMathUtils() = delete;

public:
	static void InitializeRandom();
	static float RandomFloat(float a, float b);  // Generate random float number in range [a,b]
	static float Avg(const std::vector<float>& values); // Calculate average value of given vector
	static float DegreesToRadians(float degrees); // Conver degrees to radians
	static float RadiansToDegrees(float radians); // Conver radians to degrees
};

}
