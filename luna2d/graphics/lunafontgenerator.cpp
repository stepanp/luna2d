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

#include "lunafontgenerator.h"

using namespace luna2d;

LUNAFontGenerator::~LUNAFontGenerator()
{
	if(face) FT_Done_Face(face);
	if(library) FT_Done_FreeType(library);
}

bool LUNAFontGenerator::Load(const std::string& filename, LUNAFileLocation location)
{
	// Initialize FreeType
	FT_Error error = FT_Init_FreeType(&library);
	if(error) return false;

	// Load font to FreeType
	auto buffer = LUNAEngine::SharedFiles()->ReadFile(filename, location);
	error = FT_New_Memory_Face(library, &buffer[0], buffer.size(), 0, &face);
	if(error) return false;

	// Set font size
	FT_Set_Char_Size(face, 14 * 64, 14 * 64, 96, 96);

	// Render char symbols to image
	std::string chars = "QWERTY";
	LUNAImage image(256, 256, LUNAColorType::RGBA);
	int offset = 0;
	for(char c : chars)
	{
		error = FT_Load_Char(face, c, FT_LOAD_RENDER);
		if(error)
		{
			LUNA_LOGE("%d", error);
			continue;
		}

		FT_Bitmap bmp = face->glyph->bitmap;
		if(bmp.pixel_mode != FT_PIXEL_MODE_GRAY)
		{
			LUNA_LOGE("Supported only FT_PIXEL_MODE_GRAY");
			continue;
		}

		int width = bmp.width;
		int height = bmp.rows;
		LUNAImage charImg(width, height, LUNAColorType::RGBA);
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				// FreeType generates 8-bit per pixel grayscale bitmap
				// We convert it to 32-bit image with alpha channel
				// Brightness level of pixels is using as alpha channel
				unsigned char bmpPixel = bmp.buffer[x * y];
				charImg.SetPixel(x, y, LUNAColor::Rgb(255, 255, 255, bmpPixel));
			}
		}

		image.DrawImage(offset, 0, charImg);
		offset += width + 1;
	}

	if(image.IsEmpty()) return false;

	// Create texture from image
	texture = std::make_shared<LUNATexture>(image);

	return true;
}

std::shared_ptr<LUNAFont> LUNAFontGenerator::CreateFont(int size)
{
	return nullptr;
}

