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

#include "lunajpegformat.h"
#include "lunalog.h"
#include <jpeglib.h>
#include <setjmp.h>

using namespace luna2d;

const int DEFAULT_JPEG_QUALITY = 90;
const size_t JPEG_OUTPUT_BUFFER_SIZE = 4096;

struct LUNAJpegError
{
	jpeg_error_mgr mgr;
	jmp_buf jmpBuffer;
};

struct LUNAJpegDestination
{
	jpeg_destination_mgr mgr;
	std::vector<unsigned char>* data;
};


// SEE: "LUNAImageFormat::Decode"
bool LUNAJpegFormat::Decode(const std::vector<unsigned char>& inData, std::vector<unsigned char>& outData,
	int& outWidth, int& outHeight, LUNAColorType& outColorType) const
{
	return false;
}

// SEE: "LUNAImageFormat::Encode"
bool LUNAJpegFormat::Encode(const std::vector<unsigned char>& inData, std::vector<unsigned char>& outData,
	int width, int height, LUNAColorType colorType) const
{
	jpeg_compress_struct info;

	// Set up error handling
	LUNAJpegError jerr;
	info.err = jpeg_std_error(&jerr.mgr);
	info.err->error_exit = [](j_common_ptr info)
	{
		LUNAJpegError* err = reinterpret_cast<LUNAJpegError*>(info->err);
		longjmp(err->jmpBuffer, 1);
	};

	if(setjmp(jerr.jmpBuffer))
	{
		jpeg_destroy_compress(&info);
		return false;
	}

	jpeg_create_compress(&info);

	// Set up helpers for writing directly to output buffer "outData"
	LUNAJpegDestination dest;
	dest.data = &outData;

	info.dest = &dest.mgr;
	info.dest->init_destination = [](j_compress_ptr info)
	{
		LUNAJpegDestination* dest = reinterpret_cast<LUNAJpegDestination*>(info->dest);

		dest->data->resize(JPEG_OUTPUT_BUFFER_SIZE);
		info->dest->next_output_byte = dest->data->data();
		info->dest->free_in_buffer = dest->data->size();
	};

	info.dest->term_destination = [](j_compress_ptr info)
	{
		LUNAJpegDestination* dest = reinterpret_cast<LUNAJpegDestination*>(info->dest);

		dest->data->resize(dest->data->size() - info->dest->free_in_buffer);
	};

	info.dest->empty_output_buffer = [](j_compress_ptr info) -> boolean
	{
		LUNAJpegDestination* dest = reinterpret_cast<LUNAJpegDestination*>(info->dest);

		size_t oldSize = dest->data->size();
		dest->data->resize(oldSize + JPEG_OUTPUT_BUFFER_SIZE);
		info->dest->next_output_byte = dest->data->data() + oldSize;
		info->dest->free_in_buffer = JPEG_OUTPUT_BUFFER_SIZE;

		return true;
	};

	info.image_width = width;
	info.image_height = height;

	if(colorType == LUNAColorType::RGBA || colorType == LUNAColorType::RGB)
	{
		info.input_components = 3;
		info.in_color_space = JCS_RGB;
	}
	else if(colorType == LUNAColorType::ALPHA)
	{
		info.input_components = 1;
		info.in_color_space = JCS_GRAYSCALE;
	}
	else
	{
		LUNA_LOGE("Unsupported jpeg color type \"%s\"", COLOR_TYPE.FromEnum(colorType).c_str());
		jpeg_destroy_compress(&info);
		return false;
	}

	jpeg_set_defaults(&info);
	jpeg_set_quality(&info, DEFAULT_JPEG_QUALITY, true);
	jpeg_start_compress(&info, true);

	if(colorType == LUNAColorType::RGBA)
	{
		std::vector<unsigned char> tempRows(info.image_width * 3);

		JSAMPROW rowPointer[1];
		rowPointer[0] = &tempRows[0];

		while(info.next_scanline < info.image_height)
		{
			size_t rowOffset = info.next_scanline * info.image_width * 4;

			// Input strides by 4 bytes(RGBA), output strides by 3 bytes(RGB)
			for(int i = 0, j = 0; i < info.image_width * 4; i += 4, j += 3)
			{
				tempRows[j + 0] = inData[rowOffset + i + 0];
				tempRows[j + 1] = inData[rowOffset + i + 1];
				tempRows[j + 2] = inData[rowOffset + i + 2];
			}

			jpeg_write_scanlines(&info, rowPointer, 1);
		}
	}
	else
	{
		JSAMPROW rowPointer[1];
		size_t rowStride = info.image_width * info.input_components;

		while(info.next_scanline < info.image_height)
		{
			rowPointer[0] = (const_cast<unsigned char*>(&inData[info.next_scanline * rowStride]));
			jpeg_write_scanlines(&info, rowPointer, 1);
		}
	}

	jpeg_finish_compress(&info);
	jpeg_destroy_compress(&info);

	return true;
}
