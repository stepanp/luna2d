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
#include "lunasizes.h"
#include "lunamath.h"

using namespace luna2d;

struct CharRegion
{
	CharRegion(char c, int x, int y, int width, int height) : c(c), x(x), y(y), width(width), height(height) {}

	char c = '\0';
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
};

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
	fontBuffer = std::move(LUNAEngine::SharedFiles()->ReadFile(filename, location));
	error = FT_New_Memory_Face(library, &fontBuffer[0], fontBuffer.size(), 0, &face);
	if(error) return false;

	return true;
}

std::shared_ptr<LUNAFont> LUNAFontGenerator::GenerateFont(int size)
{
	if(!face) return nullptr;

	// For same font size on all resolutions size
	// scale font size to  virtual screen resolution and sets default DPI
	int fontSize = size / LUNAEngine::SharedSizes()->GetTextureScale();
	FT_Set_Char_Size(face, fontSize * 64, fontSize * 64, 96, 96);

	// All available chars
	std::string chars = "?qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[];',./1234567890!@#$%^&*()-+= ";

	// Calculate texture size
	int charArea = fontSize * fontSize;
	int totalArea = (chars.size() + 1) * charArea;
	int textureSide = math::NearestPowerOfTwo(std::ceil(std::sqrt(totalArea)));
	LUNAImage image(textureSide, textureSide, LUNAColorType::RGBA);

	std::vector<CharRegion> charRegions;

	// Render chars to texture
	int offsetX = 0;
	int offsetY = 0;
	FT_Error error;
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
				unsigned char bmpPixel = bmp.buffer[x + width * y];
				charImg.SetPixel(x, y, LUNAColor::Rgb(255, 255, 255, bmpPixel));
			}
		}

		if(offsetX + charImg.GetWidth() > image.GetWidth())
		{
			offsetY += height;
			offsetX = 0;
		}
		image.DrawImage(offsetX, offsetY, charImg);

		charRegions.push_back(CharRegion(c, offsetX, offsetY, width, height));

		offsetX += width;
	}

	if(image.IsEmpty()) return nullptr;

	// Create font
	auto font = std::make_shared<LUNAFont>(std::make_shared<LUNATexture>(image), size);

	// Set texture regions for chars
	for(auto region : charRegions) font->SetCharRegion(region.c, region.x, region.y, region.width, region.height);
	font->SetUnknownCharRegion(charRegions[0].x, charRegions[0].y, charRegions[0].width, charRegions[0].height);

	return font;
}

