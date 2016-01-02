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

#include <stddef.h> // For size_t
#include <vector>

namespace luna2d{

//---------------------------
// Color type of image format
//---------------------------
enum class LUNAColorType
{
	RGB = 1,
	RGBA = 2,
};

//---------------------------
// Interface for image format
//---------------------------
class LUNAImageFormat
{
public:
	virtual ~LUNAImageFormat() {}

public:
	// Decode raw image data to uncompressed bitmap data
	// Params:
	// "inData" - Input image data buffer
	// "outData" - Output decoded data buffer
	// "outWidth" - Output width of image
	// "outHeight" - Output height of image
	// "outColorType" - Output color type of image
	virtual bool Decode(const std::vector<unsigned char>& inData, std::vector<unsigned char>& outData,
		int& outWidth, int& outHeight, LUNAColorType& outColorType) const = 0;
};

}
