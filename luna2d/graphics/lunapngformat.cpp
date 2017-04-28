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

#include "lunapngformat.h"
#include "lunalog.h"

using namespace luna2d;

//-----------------------------------------------------
// Helper to pass data to "ReadPngFromBuffer" function
//-----------------------------------------------------
struct LUNAPngData
{
	const unsigned char* data;
	size_t offset;
};

// Callback for read png from buffer instead of file
static void ReadPngFromBuffer(png_structp pngPtr, png_bytep data, png_size_t size)
{
	LUNAPngData* pngData = static_cast<LUNAPngData*>(png_get_io_ptr(pngPtr));
	memcpy(data, pngData->data + pngData->offset, size);
	pngData->offset += size;
}

// Callback for write png to buffer instead of file
static void WritePngToBuffer(png_structp pngPtr, png_bytep data, png_size_t size)
{
	std::vector<unsigned char>* outData = static_cast<std::vector<unsigned char>*>(png_get_io_ptr(pngPtr));
	outData->insert(outData->end(), data, data + size);
}

// SEE: "LUNAImageFormat::Decode"
bool LUNAPngFormat::Decode(const std::vector<unsigned char>& inData, std::vector<unsigned char>& outData,
	int& outWidth, int& outHeight, LUNAColorType& outColorType) const
{
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(!pngPtr) return false;

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, nullptr, nullptr);
		return false;
	}

	// Set custrom function to reading file
	LUNAPngData pngData;
	pngData.data = &inData[0];
	pngData.offset = 0;
	png_set_read_fn(pngPtr, &pngData, &ReadPngFromBuffer);

	// Read image params
	png_read_info(pngPtr, infoPtr);
	int width = png_get_image_width(pngPtr, infoPtr);
	int height = png_get_image_height(pngPtr, infoPtr);

	// Read color type to engine format
	png_uint_32 pngColorType = png_get_color_type(pngPtr, infoPtr);
	if(pngColorType == PNG_COLOR_TYPE_RGB) outColorType = LUNAColorType::RGB;
	else if(pngColorType == PNG_COLOR_TYPE_RGBA) outColorType = LUNAColorType::RGBA;
	else
	{
		LUNA_LOGE("Unsupported png color type");
		png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
		return false;
	}

	png_size_t rowBytes = png_get_rowbytes(pngPtr, infoPtr); // Size of row in bytes
	png_bytepp rowPointers = new png_bytep[height]; // Pointers to start of ech row

	// Prepare output data buffer
	size_t dataSize = rowBytes * height;
	outData.clear();
	outData.resize(dataSize);

	// Set pointers to read decompressed image to output data buffer
	for(int i = 0; i < height; i++)
	{
		rowPointers[i] = &outData[0] + (i * rowBytes);
	}

	// Read image data to output buffer by row pointers
	png_read_image(pngPtr, rowPointers);
	png_read_end(pngPtr, nullptr);

	// Cleanup
	png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
	delete[] rowPointers;

	// Return values
	outWidth = width;
	outHeight = height;

	return true;
}

// SEE: "LUNAImageFormat::Encode"
bool LUNAPngFormat::Encode(const std::vector<unsigned char>& inData, std::vector<unsigned char>& outData,
	int width, int height, LUNAColorType colorType) const
{
	png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(!pngPtr) return false;

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr)
	{
		png_destroy_write_struct(&pngPtr, nullptr);
		return false;
	}

	if(setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_write_struct(&pngPtr, &infoPtr);
		return false;
	}

	// Set custrom function to writing file
	png_set_write_fn(pngPtr, &outData, &WritePngToBuffer, nullptr);

	png_uint_32 pngColorType = PNG_COLOR_TYPE_RGBA;
	switch(colorType)
	{
	case LUNAColorType::RGB:
		pngColorType = PNG_COLOR_TYPE_RGB;
		break;
	case LUNAColorType::RGBA:
		pngColorType = PNG_COLOR_TYPE_RGBA;
		break;
	default:
		{
			LUNA_LOGE("Unsupported png color type");
			png_destroy_write_struct(&pngPtr, &infoPtr);
			return false;
		}
	}

	outData.clear();

	png_set_IHDR(pngPtr, infoPtr, width, height, 8, pngColorType,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	std::vector<png_bytep> rows(height);
	for (int i = 0; i < height; i++)
	{
		rows[i] = const_cast<png_bytep>(&inData[i * width * GetBytesPerPixel(colorType)]);
	}

	png_write_info(pngPtr, infoPtr);
	png_write_image(pngPtr, &rows[0]);
	png_write_end(pngPtr, infoPtr);

	png_destroy_write_struct(&pngPtr, &infoPtr);

	return true;
}
