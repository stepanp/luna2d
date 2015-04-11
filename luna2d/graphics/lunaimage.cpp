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

int BytesPerPixel(LUNAColorType colorType)
{
	return colorType == LUNAColorType::RGBA ? 4 : 3;
}

// Construct empty image
LUNAImage::LUNAImage() : LUNAImage(0, 0, LUNAColorType::RGBA)
{
}

// Constuct empty image with given sizes and color type
LUNAImage::LUNAImage(int width, int height, LUNAColorType colorType) :
	data(width * height * BytesPerPixel(colorType)),
	width(width),
	height(height),
	colorType(colorType)
{
}

// Constuct image from given data
LUNAImage::LUNAImage(int width, int height, LUNAColorType colorType, std::vector<unsigned char> data) :
	data(data),
	width(width),
	height(height),
	colorType(colorType)
{
}

// Constructor with loading
LUNAImage::LUNAImage(const std::string& filename, const LUNAImageFormat& format, LUNAFileLocation location) : LUNAImage()
{
	Load(filename, format, location);
}

bool LUNAImage::IsEmpty() const
{
	return data.empty();
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

void LUNAImage::SetPixel(int x, int y, const LUNAColor& color)
{
	if(IsEmpty() || x < 0 || y < 0 || x > width || y > height) return;

	int pos = x * y * BytesPerPixel(colorType);
	unsigned char r = color.r * 255;
	unsigned char g = color.g * 255;
	unsigned char b = color.b * 255;

	data[pos] = r;
	data[pos + 1] = g;
	data[pos + 2] = b;

	if(colorType == LUNAColorType::RGBA) data[pos + 3] = color.a * 255;
}

LUNAColor LUNAImage::GetPixel(int x, int y) const
{
	if(IsEmpty() || x < 0 || y < 0 || x > width || y > height) return LUNAColor::RgbFloat(1, 0, 1);

	int pos = x * y * BytesPerPixel(colorType);
	return LUNAColor::Rgb(data[pos], data[pos + 1], data[pos + 2], colorType == LUNAColorType::RGBA ? data[pos + 3] : 255);
}

// Draw another image to this image
void LUNAImage::DrawImage(int x, int y, const LUNAImage& image)
{
	if(IsEmpty() || image.IsEmpty()) return;
	if(x < 0 || y < 0 || x + image.GetWidth() > width || y + image.GetHeight() > height) return;

	auto data = image.GetData();
	for(int i = x; i < image.GetWidth(); i++)
	{
		for(int j = y; j < image.GetHeight(); j++)
		{
			SetPixel(x, y, image.GetPixel(i, j));
		}
	}
}
