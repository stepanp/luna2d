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

#pragma once

#include "lunautf.h"
#include "lunafont.h"

#define generic _FREETYPE_GENERIC // Avoid conflict with C++/CX keyword
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H
#undef generic

namespace luna2d{

const std::u32string LATIN_CHARS = LUNA_UTF32("qwertyyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
const std::u32string CYRILLIC_CHARS = LUNA_UTF32("йцукенгшщзхъфывапролджэячсмитьбюёЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁ");
const std::u32string COMMON_CHARS = LUNA_UTF32(" !@#$%^&*()-+=!№?<>[]{}:;,.\\/|`~'\"_");
const std::u32string NUMBER_CHARS = LUNA_UTF32("1234567890");
const int CHAR_PADDING = 2; // Size of padding between chars(in pixels)

//----------------------------------------------
// Util for generate bitmap fonts using FreeType
//----------------------------------------------
class LUNAFontGenerator
{
public:
	~LUNAFontGenerator();

private:
	FT_Library library = nullptr;
	FT_Face face = nullptr;
	std::vector<unsigned char> fontBuffer;

	bool enableLatin = true;
	bool enableCyrillic = true;
	bool enableCommon = true;
	bool enableNumbers = true;

private:
	// Conversions between pixels and internal FreeType units
	int UnitsToPixels(int units);
	int PixelsToUnits(int pixels);

public:
	bool Load(const std::string& filename, LUNAFileLocation location = LUNAFileLocation::ASSETS); // Load
	std::shared_ptr<LUNAFont> GenerateFont(int size); // Create bitmap font with given size
};

}
