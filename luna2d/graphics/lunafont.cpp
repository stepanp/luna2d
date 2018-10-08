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

#include "lunafont.h"
#include "lunautf.h"

using namespace luna2d;

LUNAFont::LUNAFont(const std::shared_ptr<LUNATexture>& texture, int size, int outlineSize = 0) :
	texture(texture),
	size(size),
	outlineSize(outlineSize)
{
}

std::weak_ptr<LUNATexture> LUNAFont::GetTexture()
{
	return texture;
}

 // Set texture region for given char
void LUNAFont::SetGlyph(char32_t c, const LUNAGlyph& glyph)
{
	glyphs[c] = glyph;
}

 // Set texture region for unknown char
void LUNAFont::SetUnknownCharGlyph(const LUNAGlyph& glyph)
{
	unknownChar = glyph;
}

const LUNAGlyph& LUNAFont::GetGlyphForChar(char32_t c)
{
	if(glyphs.count(c) == 0) return unknownChar; // If char not found return unknown char glyph
	return glyphs[c];
}

int LUNAFont::GetSize()
{
	return size;
}

int LUNAFont::GetOutlineSize()
{
	return outlineSize;
}

// Get width of one-line string typed with this font
float LUNAFont::GetStringWidth(const std::string& string)
{
	float width = 0.0f;

	for(auto c : utf::ToUtf32(string)) width += GetGlyphForChar(c).width;

	return width;
}

// Get height of one-line string typed with this font
float LUNAFont::GetStringHeight(const std::string& string)
{
	float maxHeight = 0.0f;

	for(auto c : utf::ToUtf32(string)) maxHeight = std::max(maxHeight, GetGlyphForChar(c).height);

	return maxHeight;
}
