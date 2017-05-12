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

#pragma once

#include "lunafiles.h"
#include "lunapngformat.h"
#include "lunacolor.h"
#include "lunablendingmode.h"
#include "lunaassets.h"

namespace luna2d{

class LUNATexture;

//------------------------------
// Class for working with images
//------------------------------
class LUNAImage : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNAImage)

public:
	// Construct empty image
	LUNAImage();

	// Constuct empty image with given sizes and color type
	LUNAImage(int width, int height, LUNAColorType colorType);

	// Constuct image from given data
	LUNAImage(int width, int height, LUNAColorType colorType, const std::vector<unsigned char>& data);

	// Constuct image from given data without copy
	LUNAImage(int width, int height, LUNAColorType colorType, std::vector<unsigned char>&& data);

	// Constructor with loading
	LUNAImage(const std::string& filename, const LUNAImageFormat& format,
		LUNAFileLocation location = LUNAFileLocation::ASSETS);

private:
	std::vector<unsigned char> data;
	int width, height;
	LUNAColorType colorType;

private:
	// Convert given coordinates to position in data buffer
	int CoordsToPos(int x, int y) const;

	// Draw another image to this image without blending
	void BlendNone(int x, int y, const LUNAImage& image);

	// Draw another image to this image with alpha blending
	void BlendAlpha(int x, int y, const LUNAImage& image);

	uint32_t GetBytePixel(size_t pos);

	void SetBytePixel(size_t pos, uint32_t color);

public:
	bool IsEmpty() const;
	const std::vector<unsigned char>& GetData() const;
	int GetWidth() const;
	int GetHeight() const;
	LUNAColorType GetColorType() const;

	// Load image from file
	bool Load(const std::string& filename, const LUNAImageFormat& format,
		LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Save image to file
	bool Save(const std::string& filename, const LUNAImageFormat& format,
		LUNAFileLocation location = LUNAFileLocation::APP_FOLDER);

	// Set pixmap size without stretching image
	void SetSize(int width, int height);

	void SetPixel(int x, int y, const LUNAColor& color);
	LUNAColor GetPixel(int x, int y) const;

	// Fill image with given color
	void Fill(const LUNAColor& color);

	// Fill rectangle on image with given color
	void FillRectangle(int x, int y, int width, int height, const LUNAColor& color);

	// Draw another image to this image
	void DrawImage(int x, int y, const LUNAImage& image, LUNABlendingMode blendingMode);

	// Draw image from given buffer to this image
	// Width and height of buffer in pixels
	// Color type of given buffer should be same as image color type
	void DrawBuffer(int x, int y,
		const std::vector<unsigned char>& buffer, int bufferWidth, int bufferHeight, LUNAColorType bufferColorType);

	// Draw image from given raw buffer to this image
	// Width and height of buffer in pixels
	// Color type of given buffer should be same as image color type
	void DrawRawBuffer(int x, int y,
		const unsigned char* buffer, int bufferWidth, int bufferHeight, LUNAColorType bufferColorType);

	// Flip image vertically
	void FlipVertically();

	// Flip image horizontally
	void FlipHorizontally();
};

}
