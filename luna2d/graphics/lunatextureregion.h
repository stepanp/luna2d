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

#include "lunatexture.h"

namespace luna2d{

class LUNATextureRegion : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNATextureRegion)

public:
	LUNATextureRegion(const std::weak_ptr<LUNATexture>& texture, float u1, float v1, float u2, float v2);
	LUNATextureRegion(const std::weak_ptr<LUNATexture>& texture, int x, int y, int width, int height);

private:
	std::weak_ptr<LUNATexture> texture;
	float u1, v1, u2, v2;

public:
	std::weak_ptr<LUNATexture> GetTexture();
	float GetU1();
	float GetV1();
	float GetU2();
	float GetV2();

	// Get sizes in pixels
	float GetWidth();
	float GetHeight();

	// Get sizes in game points (scaled to virtual resoultion)
	float GetWidthPoints();
	float GetHeightPoints();
};

}
