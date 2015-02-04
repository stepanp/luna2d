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

#include "lunapngformat.h"
#include "lunalog.h"

using namespace luna2d;

// Callback for read png from buffer instead of file
static void ReadPngFromBuffer(png_structp pngPtr, png_bytep data, png_size_t size)
{
	LUNAPngData *pngData = static_cast<LUNAPngData*>(png_get_io_ptr(pngPtr));
	memcpy(data, pngData->data + pngData->offset, size);
	pngData->offset += size;
}

// SEE: "LUNAImageFormat::Decode"
bool LUNAPngFormat::Decode(unsigned char *inData, size_t inSize,
		unsigned char **outData, size_t *outSize, int *outWidth, int *outHeight, LUNAColorType *outColorType)
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
	pngData.data = inData;
	pngData.size = inSize;
	pngData.offset = 0;
	png_set_read_fn(pngPtr, &pngData, &ReadPngFromBuffer);

	// Read image params
	png_read_info(pngPtr, infoPtr);
	int width = png_get_image_width(pngPtr, infoPtr);
	int height = png_get_image_height(pngPtr, infoPtr);

	// Read color type to engine format
	png_uint_32 pngColorType = png_get_color_type(pngPtr, infoPtr);
	if(pngColorType == PNG_COLOR_TYPE_RGB) *outColorType = LUNAColorType::RGB;
	else if(pngColorType == PNG_COLOR_TYPE_RGBA) *outColorType = LUNAColorType::RGBA;
	else
	{
		LUNA_LOGE("Unsupported png color type");
		png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
		return false;
	}

	// Read image data
	png_size_t rowBytes = png_get_rowbytes(pngPtr, infoPtr); // Size of row in bytes
	png_bytepp rowPointers = new png_bytep[height]; // Pointers to start of ech row

	size_t dataSize = rowBytes * height;
	unsigned char *data = new unsigned char[dataSize];

	// Set pointers to read decompressed image to "data" buffer
	for(int i = 0;i < height;i++) rowPointers[i] = data + (i * rowBytes);

	png_read_image(pngPtr, rowPointers);
	png_read_end(pngPtr, nullptr);

	// Cleanup
	png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
	delete[] rowPointers;

	// Return values
	*outData = data;
	*outSize = dataSize;
	*outWidth = width;
	*outHeight = height;

	return true;
}
