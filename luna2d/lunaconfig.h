//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

enum class LUNAOrientation
{
	PORTRAIT,
	LANDSCAPE
};

enum class LUNAScaleMode
{
	FIT_TO_HEIGHT_LEFT,
	FIT_TO_HEIGHT_RIGHT,
	FIT_TO_HEIGHT_CENTER,
	FIT_TO_WIDTH_TOP,
	FIT_TO_WIDTH_BOTTOM,
	FIT_TO_WIDTH_CENTER,
};

class LUNAConfig
{
public:
	LUNAOrientation orientation = LUNAOrientation::LANDSCAPE;
	LUNAScaleMode scaleMode = LUNAScaleMode::FIT_TO_HEIGHT_LEFT;
	std::vector<std::string> resolutions = { DEFAULT_RESOLUTION };
	int baseWidth = 480;
	int baseHeight = BASE_SIZE;

public:
	void Read();
};

}
