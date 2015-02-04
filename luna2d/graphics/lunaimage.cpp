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

#include "lunaimage.h"
#include "lunaengine.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunatexture.h"

using namespace luna2d;

LUNAImage::LUNAImage() :
	width(0),
	height(0),
	colorType(LUNAColorType::RGBA)
{
}

const std::vector<unsigned char>& LUNAImage::GetData() const
{
	return data;
}

int LUNAImage::GetWidth() const
{
	return width;
}

int LUNAImage::GetHeight() const
{
	return height;
}

LUNAColorType LUNAImage::GetColorType() const
{
	return colorType;
}

bool LUNAImage::Load(const std::string& filename, const LUNAImageFormat& format, LUNAFileLocation location)
{
	// Read data form file
	std::vector<unsigned char> fileData = LUNAEngine::SharedFiles()->ReadFile(filename, location);
	if(fileData.empty()) return false;

	return format.Decode(fileData, data, width, height, colorType);
}
