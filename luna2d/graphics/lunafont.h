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

#pragma once

#include "lunatextureregion.h"

namespace luna2d{

class LUNAFont : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNAFont)

public:
	LUNAFont(const std::shared_ptr<LUNATexture>& texture, int size);

private:
	std::shared_ptr<LUNATexture> texture;
	std::unordered_map<char32_t, std::shared_ptr<LUNATextureRegion>> chars;
	std::shared_ptr<LUNATextureRegion> unknownChar;
	int size;

public:
	void SetCharRegion(char32_t c, int x, int y, int width, int height); // Set texture region for given char
	void SetUnknownCharRegion(int x, int y, int width, int height); // Set texture region for unknown char

	std::weak_ptr<LUNATextureRegion> GetRegionForChar(char32_t c);
	int GetSize();
};

}
