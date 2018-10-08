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

#include "lunafontgenerator.h"
#include "lunasizes.h"
#include "lunamath.h"

using namespace luna2d;

const std::u32string LATIN_CHARS = LUNA_UTF32("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
const std::u32string DIACTRITIC_CHARS = LUNA_UTF32("áàâäåãéèêîíìôöóòûùúüœæøßçñÁÀÂÄÅÃÃÉÈÊÎÍÌÔÖÓÒÛÙÚÜŒÆØÇÑ");
const std::u32string CYRILLIC_CHARS = LUNA_UTF32("йцукенгшщзхъфывапролджэячсмитьбюёЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁ");
const std::u32string COMMON_CHARS = LUNA_UTF32(" !@#$%^&*()-+=!№?¿<>[]{}:;,.\\/|`~'\"_©");
const std::u32string NUMBER_CHARS = LUNA_UTF32("1234567890");

struct CharRegion
{
	CharRegion(char32_t c, int regionX, int regionY, int regionWidth, int regionHeight,
		float charWidth, float charHeight, float charOffsetX, float charOffsetY): c(c),
		regionX(regionX), regionY(regionY), regionWidth(regionWidth), regionHeight(regionHeight),
		charWidth(charWidth), charHeight(charHeight), charOffsetX(charOffsetX), charOffsetY(charOffsetY) {}

	char32_t c = '\0';
	int regionX;
	int regionY;
	int regionWidth;
	int regionHeight;
	float charWidth;
	float charHeight;
	float charOffsetX;
	float charOffsetY;

	LUNAGlyph ToGlyph(const std::shared_ptr<LUNATexture>& texture)
	{
		float textureScale = LUNAEngine::SharedSizes()->GetTextureScale();

		return LUNAGlyph(std::make_shared<LUNATextureRegion>(texture, regionX, regionY, regionWidth, regionHeight),
			charWidth * textureScale, charHeight * textureScale, charOffsetX * textureScale, charOffsetY * textureScale);
	}
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
	enableDiactritic = true;
	enableCyrillic = true;
	enableCommon = true;
	enableNumbers = true;
	customSymbols.clear();
	outlineSize = 0;
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
	if(enableDiactritic) chars += DIACTRITIC_CHARS;
	if(enableCyrillic) chars += CYRILLIC_CHARS;
	if(enableCommon) chars += COMMON_CHARS;
	if(enableNumbers) chars += NUMBER_CHARS;
	if(!customSymbols.empty()) chars += customSymbols;
	int outlineSizePixels = std::floor(outlineSize / LUNAEngine::SharedSizes()->GetTextureScale());
	bool enableOutline = outlineSizePixels > 0;

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
	image.FillRectangle(0, 0, maxW - 1, maxH, LUNAColor::WHITE);
	CharRegion unknownChar('\0', 0, 0, maxW, maxH, maxW, maxH, 0.0f, 0.0f);

	std::vector<CharRegion> charRegions;
	int penX = maxW;
	int penY = 0;

	// Draw chars on image
	for(char32_t c : chars)
	{
		FT_Error error = FT_Load_Char(face, c, enableOutline ? FT_LOAD_DEFAULT : FT_LOAD_RENDER);
		if(error) continue;

		FT_Stroker stroker;
		FT_Glyph glyph;
		FT_Bitmap bmp;

		if(enableOutline)
		{
			FT_Stroker_New(library, &stroker);
			FT_Stroker_Set(stroker, PixelsToUnits(outlineSizePixels), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
			FT_Get_Glyph(face->glyph, &glyph);
			FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
			FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
			bmp = reinterpret_cast<FT_BitmapGlyph>(glyph)->bitmap;
		}
		else
		{
			bmp = face->glyph->bitmap;
		}

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

		int regionX = penX;
		int regionY = penY;
		int regionWidth = bmp.width;
		int regionHeight = bmp.rows;

		// Draw char bimtap to image
		image.DrawRawBuffer(regionX, regionY, bmp.buffer, regionWidth, regionHeight, LUNAColorType::ALPHA);

		// SEE: https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html

		int charWidth = UnitsToPixels(face->glyph->advance.x);
		int charHeight = maxH;
		int charOffsetX = UnitsToPixels(face->glyph->metrics.horiBearingX) - outlineSizePixels;
		int charOffsetY = baseline + UnitsToPixels(face->glyph->metrics.horiBearingY - face->glyph->metrics.height) - outlineSizePixels;

		charRegions.push_back(CharRegion(c, regionX, regionY, regionWidth, regionHeight,
			charWidth, charHeight, charOffsetX, charOffsetY));

		penX += regionWidth + CHAR_PADDING;

		if(enableOutline)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Glyph(glyph);
		}
	}

	if(image.IsEmpty()) return nullptr;

	// Crop empty space in texture if possible
	int croppedHeight = math::NearestPowerOfTwo(penY + maxH);
	if(croppedHeight < image.GetHeight()) image.SetSize(image.GetWidth(), croppedHeight);

	// Create texture from generated image
	auto texture = std::make_shared<LUNATexture>(image);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Cache generated texture to APP_DATA folder for reloading when lossing GL context
	texture->Cache(image.GetData());
#endif

	// Create font
	auto font = std::make_shared<LUNAFont>(texture, size, outlineSize);

	// Set texture regions for chars
	for(auto region : charRegions) font->SetGlyph(region.c, region.ToGlyph(texture));
	font->SetUnknownCharGlyph(unknownChar.ToGlyph(texture));

	return font;
}

