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

#include "lunatextureregion.h"

namespace luna2d{

struct LUNAGlyph
{
	LUNAGlyph(const std::shared_ptr<LUNATextureRegion>& region, float width, float height, float offsetX, float offsetY) :
		region(region), width(width), height(height), offsetX(offsetX), offsetY(offsetY) {}

	LUNAGlyph() {}

	std::shared_ptr<LUNATextureRegion> region;
	float width = 0.0f;
	float height = 0.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
};


class LUNAFont : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNAFont)

public:
	LUNAFont(const std::shared_ptr<LUNATexture>& texture, int size, int outlineSize);

private:
	std::shared_ptr<LUNATexture> texture;
	std::unordered_map<char32_t, LUNAGlyph> glyphs;
	LUNAGlyph unknownChar;
	int size;
	int outlineSize;

public:
	std::weak_ptr<LUNATexture> GetTexture();

	void SetGlyph(char32_t c, const LUNAGlyph& glyph); // Set texture region for given char
	void SetUnknownCharGlyph(const LUNAGlyph& glyph); // Set texture region for unknown char

	const LUNAGlyph& GetGlyphForChar(char32_t c);
	int GetSize();
	int GetOutlineSize();

	float GetStringWidth(const std::string& string); // Get width of one-line string typed with this font
	float GetStringHeight(const std::string& string); // Get height of one-line string typed with this font
};

}
