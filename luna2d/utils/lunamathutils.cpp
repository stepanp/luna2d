//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#include "lunamathutils.h"
#include <stdlib.h>
#include <time.h>

using namespace luna2d;

void LUNAMathUtils::InitializeRandom()
{
	srand((unsigned int)time(0));
}

// Generate random float number in range [a,b]
float LUNAMathUtils::RandomFloat(float a, float b)
{
	float percent = (float)(rand() % 100) / 100;
	return a + (b - a) * percent;
}

// Calculate average value of given vector
float LUNAMathUtils::Avg(const std::vector<float>& values)
{
	float sum = 0;
	for(float value : values) sum += value;
	return sum / values.size();
}

// Convert degrees to radians
float LUNAMathUtils::DegreesToRadians(float degrees)
{
	return degrees * DEGREES_TO_RADIANS;
}

// Convert radians to degrees
float LUNAMathUtils::RadiansToDegrees(float radians)
{
	return radians * RADIANS_TO_DEGREES;
}
