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

#include "lunafont.h"
#include "lunautf.h"

using namespace luna2d;

LUNAFont::LUNAFont(const std::shared_ptr<LUNATexture>& texture, int size) :
	texture(texture),
	size(size)
{
}

const std::shared_ptr<LUNATextureRegion>& LUNAFont::GetCharRegion(char32_t c)
{
	if(chars.count(c) == 0) return unknownChar; // If char not found return unknown char region
	return chars[c];
}

 // Set texture region for given char
void LUNAFont::SetCharRegion(char32_t c, int x, int y, int width, int height)
{
	chars[c] = std::make_shared<LUNATextureRegion>(texture, x, y, width, height);
}

 // Set texture region for unknown char
void LUNAFont::SetUnknownCharRegion(int x, int y, int width, int height)
{
	unknownChar = std::make_shared<LUNATextureRegion>(texture, x, y, width, height);
}

std::weak_ptr<LUNATextureRegion> LUNAFont::GetRegionForChar(char32_t c)
{
	return GetCharRegion(c);
}

int LUNAFont::GetSize()
{
	return size;
}

// Get width of one-line string typed with this font
float LUNAFont::GetStringWidth(const std::string& string)
{
	float width = 0.0f;

	for(auto c : utf::ToUtf32(string)) width += GetCharRegion(c)->GetWidthPoints();

	return width;
}

// Get height of one-line string typed with this font
float LUNAFont::GetStringHeight(const std::string& string)
{
	float maxHeight = 0.0f;

	for(auto c : utf::ToUtf32(string)) maxHeight = std::max(maxHeight, GetCharRegion(c)->GetHeight());

	return maxHeight;
}
