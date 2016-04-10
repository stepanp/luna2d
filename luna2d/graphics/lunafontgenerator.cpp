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

#include "lunafontgenerator.h"
#include "lunasizes.h"
#include "lunamath.h"

using namespace luna2d;

struct CharRegion
{
	CharRegion(char32_t c, int x, int y, int width, int height) : c(c), x(x), y(y), width(width), height(height) {}

	char32_t c = '\0';
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

// Conversions between pixels and internal FreeType units
int LUNAFontGenerator::UnitsToPixels(int units)
{
	return units / 64;
}

int LUNAFontGenerator::PixelsToUnits(int pixels)
{
	return pixels * 64;
}

void LUNAFontGenerator::ResetCharSets()
{
	enableLatin = true;
	enableCyrillic = true;
	enableCommon = true;
	enableNumbers = true;
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

	// Set charmap to UTF-32
	FT_Select_Charmap(face , ft_encoding_unicode);

	return true;
}

std::shared_ptr<LUNAFont> LUNAFontGenerator::GenerateFont(int size)
{
	if(!face) return nullptr;

	// For same font size on all resolutions size
	// scale font size to virtual screen resolution and sets default DPI
	int fontSize = std::floor(size / LUNAEngine::SharedSizes()->GetTextureScale());
	FT_Set_Char_Size(face, PixelsToUnits(fontSize), 0, 0, 0);

	// Select available chars
	std::u32string chars;

	if(enableLatin) chars += LATIN_CHARS;
	if(enableCyrillic) chars += CYRILLIC_CHARS;
	if(enableCommon) chars += COMMON_CHARS;
	if(enableNumbers) chars += NUMBER_CHARS;

	// Get global char metrics
	int maxW = UnitsToPixels(face->size->metrics.max_advance); // Max char width
	int maxH = UnitsToPixels(face->size->metrics.height); // Max char height
	int baseline = std::fabs((float)UnitsToPixels(face->size->metrics.descender)); // Distance from bottom to baseline

	// Calculate texture size
	int charArea = (maxW + CHAR_PADDING) * (maxH + CHAR_PADDING);
	int totalArea = (chars.size() + 1 /* + unknown char */) * charArea;
	int textureSide = math::NearestPowerOfTwo(std::ceil(std::sqrt(totalArea)));

	LUNAImage image(textureSide, textureSide, LUNAColorType::ALPHA);

	// Fill image with white transparent color to avoid black artefacts around chars
	image.Fill(LUNAColor::Rgb(255, 255, 255, 0));

	// Draw placeholder for unknown char
	image.FillRectangle(0, 0, maxW, maxH, LUNAColor::WHITE);

	std::vector<CharRegion> charRegions;
	int penX = maxW;
	int penY = 0;

	// Draw chars on image
	for(char32_t c : chars)
	{
		FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
		if(error) continue;

		FT_Bitmap bmp = face->glyph->bitmap;
		if(bmp.pixel_mode != FT_PIXEL_MODE_GRAY)
		{
			LUNA_LOGE("Supported only FT_PIXEL_MODE_GRAY");
			continue;
		}

		// Move pen to next line
		if(penX + maxW > image.GetWidth())
		{
			penY += maxH + CHAR_PADDING;
			penX = 0;
		}

		int bmpWidth = bmp.width;
		int bmpHeight = bmp.rows;

		// Skip all empty chars except space
		if((bmpWidth == 0 || bmpHeight == 0) && c != ' ') continue;

		int charDescender = bmpHeight - face->glyph->bitmap_top;
		int charW = UnitsToPixels(face->glyph->advance.x);
		int charX = penX + (face->glyph->bitmap_left > 0 ? face->glyph->bitmap_left : 0);
		int charY = penY + maxH - bmpHeight - baseline + charDescender;

		// Draw char bimtap to image
		image.DrawRawBuffer(charX, charY, bmp.buffer, bmpWidth, bmpHeight, LUNAColorType::ALPHA);

		charRegions.push_back(CharRegion(c, penX, penY, charW, maxH));

		penX += charW + CHAR_PADDING;
	}

	if(image.IsEmpty()) return nullptr;

	// Create texture from generated image
	auto texture = std::make_shared<LUNATexture>(image);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Cache generated texture to APP_DATA folder for reloading when lossing GL context
	std::string reloadPath = LUNAEngine::SharedAssets()->CacheTexture(image);
	texture->SetReloadPath(reloadPath);
	texture->SetCached(true);
#endif

	// Create font
	auto font = std::make_shared<LUNAFont>(texture, size);

	// Set texture regions for chars
	for(auto region : charRegions) font->SetCharRegion(region.c, region.x, region.y, region.width, region.height);
	font->SetUnknownCharRegion(0, 0, maxW, maxH);

	return font;
}

