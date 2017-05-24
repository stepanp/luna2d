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

#include "lunaimage.h"
#include "lunaengine.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunatexture.h"

using namespace luna2d;

typedef uint32_t (*ReadPixelFunc)(const std::vector<unsigned char>&, size_t);
typedef void (*WritePixelFunc)(std::vector<unsigned char>&, size_t, uint32_t);
typedef uint32_t (*BlendingFunc)(uint32_t dest, uint32_t source);

static uint32_t ReadPixelRGBA(const std::vector<unsigned char>& data, size_t pos)
{
	return (data[pos] << 24) | (data[pos + 1] << 16) | (data[pos + 2] << 8) | data[pos + 3];
}

static uint32_t ReadPixelRGB(const std::vector<unsigned char>& data, size_t pos)
{
	return (data[pos] << 24) | (data[pos + 1] << 16) | (data[pos + 2] << 8) | 255;
}

static uint32_t ReadPixelAlpha(const std::vector<unsigned char>& data, size_t pos)
{
	return 255 | 255 | 255 | data[pos];
}

static void WritePixelRGBA(std::vector<unsigned char>& data, size_t pos, uint32_t color)
{
	data[pos] = (color >> 24) & 0xFF;
	data[pos + 1] = (color >> 16) & 0xFF;
	data[pos + 2] = (color >> 8) & 0xFF;
	data[pos + 3] = color & 0xFF;
}

static void WritePixelRGB(std::vector<unsigned char>& data, size_t pos, uint32_t color)
{
	data[pos] = (color >> 24) & 0xFF;
	data[pos + 1] = (color >> 16) & 0xFF;
	data[pos + 2] = (color >> 8) & 0xFF;
}

static void WritePixelAlpha(std::vector<unsigned char>& data, size_t pos, uint32_t color)
{
	data[pos] = color & 0xFF;
}

uint32_t BlendingNone(uint32_t dest, uint32_t source)
{
	return source;
}

uint32_t BlendingAlpha(uint32_t dest, uint32_t source)
{
	uint32_t alpha = (source & 0xFF);
	uint32_t invAlpha = 255 - alpha;

	uint32_t resultR = (((((dest >> 0) & 0xFF) * invAlpha + ((source >> 0) & 0xFF) * alpha) >> 8));
	uint32_t resultG = (((((dest >> 8) & 0xFF) * invAlpha + ((source >> 8) & 0xFF) * alpha)) & ~0xFF);
	uint32_t resultB = (((((dest >> 16) & 0xFF) * invAlpha + ((source >> 16) & 0xFF) * alpha) << 8) & ~0xFFFF);
	uint32_t resultA = (((((dest >> 24) & 0xFF) * invAlpha + ((source >> 24) & 0xFF) * alpha) << 16) & ~0xFFFFFF);

	return resultR | resultG | resultB | resultA;
}

static ReadPixelFunc GetReadPixelFunc(LUNAColorType colorType)
{
	switch(colorType)
	{
	case LUNAColorType::RGBA:
		return &ReadPixelRGBA;
	case LUNAColorType::RGB:
		return &ReadPixelRGB;
	case LUNAColorType::ALPHA:
		return &ReadPixelAlpha;
	}
}

static WritePixelFunc GetWritePixelFunc(LUNAColorType colorType)
{
	switch(colorType)
	{
	case LUNAColorType::RGBA:
		return &WritePixelRGBA;
	case LUNAColorType::RGB:
		return &WritePixelRGB;
	case LUNAColorType::ALPHA:
		return &WritePixelAlpha;
	}
}

static BlendingFunc GetBlendingFunc(LUNABlendingMode blendingMode)
{
	switch(blendingMode)
	{
	case LUNABlendingMode::NONE:
		return &BlendingNone;
	case LUNABlendingMode::ALPHA:
		return &BlendingAlpha;
	default:
		LUNA_LOGE("LUNAImage is not support blending mode \"%s\"", BLENDING_MODE.FromEnum(blendingMode).c_str());
		return &BlendingNone;
	}
}


// Construct empty image
LUNAImage::LUNAImage() : LUNAImage(0, 0, LUNAColorType::RGBA)
{
}

// Constuct empty image with given sizes and color type
LUNAImage::LUNAImage(int width, int height, LUNAColorType colorType) :
	data(width * height * GetBytesPerPixel(colorType)),
	width(width),
	height(height),
	colorType(colorType)
{
}

// Constuct image from given data
LUNAImage::LUNAImage(int width, int height, LUNAColorType colorType, const std::vector<unsigned char>& data) :
	data(data),
	width(width),
	height(height),
	colorType(colorType)
{
}

// Constuct image from given data without copy
LUNAImage::LUNAImage(int width, int height, LUNAColorType colorType, std::vector<unsigned char>&& data) :
	data(std::move(data)),
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

// Convert given coordinates to position in data buffer
size_t LUNAImage::CoordsToPos(int x, int y) const
{
	return (x + y * width) * GetBytesPerPixel(colorType);
}

LUNARectInt LUNAImage::GetSourceRect(int x, int y, int width, int height) const
{
	int sourceX = x < 0 ? -x : 0;
	int sourceY = y < 0 ? -y : 0;
	int sourceWidth = width - sourceX;
	int sourceHeight = height - sourceY;

	if(this->width - x < sourceWidth) sourceWidth = this->width - x;
	if(this->height - y < sourceHeight) sourceHeight = this->height - y;

	return LUNARectInt(sourceX, sourceY, sourceWidth, sourceHeight);
}

bool LUNAImage::CheckSourceRect(int x, int y, int width, int height) const
{
	return x + width > 0 && y + height > 0 && x < this->width && y < this->height;
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

// Load image from file
bool LUNAImage::Load(const std::string& filename, const LUNAImageFormat& format, LUNAFileLocation location)
{
	// Read data form file
	std::vector<unsigned char> fileData = LUNAEngine::SharedFiles()->ReadFile(filename, location);
	if(fileData.empty()) return false;

	return format.Decode(fileData, data, width, height, colorType);
}

// Save image to file
bool LUNAImage::Save(const std::string& filename, const LUNAImageFormat& format, LUNAFileLocation location)
{
	std::vector<unsigned char> fileData;
	if(!format.Encode(data, fileData, width, height, colorType)) return false;

	return LUNAEngine::SharedFiles()->WriteFile(filename, fileData, location);
}

// Set pixmap size without stretching image
void LUNAImage::SetSize(int width, int height)
{
	if(width <= 0 || height <= 0) return;

	if(this->width == width)
	{
		data.resize(width * height * GetBytesPerPixel(colorType));

		this->width = width;
		this->height = height;
	}
	else
	{
		std::vector<unsigned char> newData(width * height * GetBytesPerPixel(colorType));
		newData.swap(data);

		int oldWidth = this->width;
		int oldHeight = this->height;

		this->width = width;
		this->height = height;

		DrawBuffer(0, 0, newData, oldWidth, oldHeight, colorType);
	}
}

void LUNAImage::SetPixel(int x, int y, const LUNAColor& color)
{
	if(IsEmpty() || x < 0 || y < 0 || x > width || y > height) return;

	auto writePixel = GetWritePixelFunc(colorType);
	writePixel(data, CoordsToPos(x, y), color.GetUint32());
}

LUNAColor LUNAImage::GetPixel(int x, int y) const
{
	if(IsEmpty() || x < 0 || y < 0 || x > width || y > height) return LUNAColor();

	auto readPixel = GetReadPixelFunc(colorType);
	return LUNAColor::Uint32(readPixel(data, CoordsToPos(x, y)));
}

// Fill image with given color
void LUNAImage::Fill(const LUNAColor& color, LUNABlendingMode blendingMode)
{
	FillRectangle(0, 0, width, height, color, blendingMode);
}

// Fill rectangle on image with given color
void LUNAImage::FillRectangle(int x, int y, int width, int height, const LUNAColor& color, LUNABlendingMode blendingMode)
{
	if(IsEmpty() || !CheckSourceRect(x, y, width, height)) return;

	uint32_t uintColor = color.GetUint32();
	auto writePixel = GetWritePixelFunc(colorType);
	auto sourceRect = GetSourceRect(x, y, width, height);

	if(blendingMode == LUNABlendingMode::NONE)
	{
		for(int j = sourceRect.x; j < sourceRect.height; j++)
		{
			for(int i = sourceRect.y; i < sourceRect.width; i++)
			{
				writePixel(data, CoordsToPos(x + i, y + j), uintColor);
			}
		}
	}

	else if(blendingMode == LUNABlendingMode::ALPHA)
	{
		auto readPixel = GetReadPixelFunc(colorType);
		auto blend = GetBlendingFunc(blendingMode);

		for(int j = sourceRect.x; j < sourceRect.height; j++)
		{
			for(int i = sourceRect.y; i < sourceRect.width; i++)
			{
				size_t pos = CoordsToPos(x + i, y + j);
				uint32_t dest = readPixel(data, pos);

				writePixel(data, pos, blend(dest, uintColor));
			}
		}
	}

	else
	{
		LUNA_LOGE("LUNAImage is not support blending mode \"%s\"", BLENDING_MODE.FromEnum(blendingMode).c_str());
	}
}

// Draw another image to this image
void LUNAImage::DrawImage(int x, int y, const LUNAImage& image, LUNABlendingMode blendingMode)
{
	if(IsEmpty() || image.IsEmpty() || !CheckSourceRect(x, y, image.GetWidth(), image.GetHeight())) return;

	// Line-by-line copying buffer is faster than per pixel drawing
	// Use it when possible
	if(image.GetColorType() == colorType && blendingMode == LUNABlendingMode::NONE)
	{
		DrawBuffer(x, y, image.GetData(), image.GetWidth(), image.GetHeight(), image.GetColorType());
		return;
	}

	auto destReadPixel = GetReadPixelFunc(colorType);
	auto sourceReadPixel = GetReadPixelFunc(image.GetColorType());
	auto writePixel = GetWritePixelFunc(colorType);
	auto blend = GetBlendingFunc(blendingMode);
	auto sourceRect = GetSourceRect(x, y, image.GetWidth(), image.GetHeight());

	for(int j = sourceRect.x; j < sourceRect.height; j++)
	{
		for(int i = sourceRect.y; i < sourceRect.width; i++)
		{
			size_t destPos = CoordsToPos(x + i, y + j);

			uint32_t dest = destReadPixel(data, destPos);
			uint32_t source = sourceReadPixel(image.GetData(), image.CoordsToPos(i, j));

			writePixel(data, destPos, blend(dest, source));
		}
	}
}

// Draw image from given buffer to this image
// Width and height of buffer in pixels
// Color type of given buffer should be same as image color type
void LUNAImage::DrawBuffer(int x, int y,
	const std::vector<unsigned char>& buffer, int bufferWidth, int bufferHeight, LUNAColorType bufferColorType)
{
	DrawRawBuffer(x, y, buffer.data(), bufferWidth, bufferHeight, bufferColorType);
}

// Draw image from given raw buffer to this image
// Width and height of buffer in pixels
// Color type of given buffer should be same as image color type
void LUNAImage::DrawRawBuffer(int x, int y,
	const unsigned char* buffer, int bufferWidth, int bufferHeight, LUNAColorType bufferColorType)
{
	if(bufferColorType != colorType) return;

	int sourceX = x < 0 ? -x : 0;
	int sourceY = y < 0 ? -y : 0;
	int sourceWidth = bufferWidth - sourceX;
	int sourceHeight = bufferHeight - sourceY;

	int destX = x < 0 ? 0 : x;
	int destY = y < 0 ? 0 : y;

	if(width - destX < sourceWidth) sourceWidth = width - destX;
	if(height - destY < sourceHeight) sourceHeight = height - destY;

	if(sourceWidth <= 0 || sourceHeight <= 0) return;

	int bytesPerPixel = GetBytesPerPixel(bufferColorType);
	int rowX = sourceX * bytesPerPixel;
	int rowLen = sourceWidth * bytesPerPixel;
	int rowFullLen = bufferWidth * bytesPerPixel;

	for(int row = 0; row < sourceHeight; row++)
	{
		int bufferPos = (row + sourceY) * rowFullLen + rowX;
		int pos = CoordsToPos(x + sourceX, y + sourceY + row);

		memcpy(&data[pos], &buffer[bufferPos], rowLen);
	}
}

// Flip image vertically
void LUNAImage::FlipVertically()
{
	int rowLen = width * GetBytesPerPixel(colorType);
	std::vector<unsigned char> tempRow(rowLen);

	for(int row = 0; row < height / 2; row++)
	{
		int pos = CoordsToPos(0, row);
		int endPos = CoordsToPos(0, height - row - 1);

		memcpy(&tempRow[0], &data[pos], rowLen);
		memcpy(&data[pos], &data[endPos], rowLen);
		memcpy(&data[endPos], &tempRow[0], rowLen);
	}
}

// Flip image horizontally
void LUNAImage::FlipHorizontally()
{
	uint32_t tempPixel;
	int pixelLen = GetBytesPerPixel(colorType);

	for(int x = 0; x < width / 2; x++)
	{
		for(int row = 0; row < height; row++)
		{
			int pos = CoordsToPos(x, row);
			int endPos = CoordsToPos(width - x - 1, row);

			memcpy(&tempPixel, &data[pos], pixelLen);
			memcpy(&data[pos], &data[endPos], pixelLen);
			memcpy(&data[endPos], &tempPixel, pixelLen);
		}
	}
}
