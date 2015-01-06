//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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
#include "lunatextureregion.h"

using namespace luna2d;

LUNATextureRegion::LUNATextureRegion(const std::weak_ptr<LUNATexture>& texture, float u1, float v1, float u2, float v2) :
	LUNAAsset(LUNAAssetType::TEXTURE_REGION), texture(texture), u1(u1), v1(v1), u2(u2), v2(v2)
{
}

LUNATextureRegion::LUNATextureRegion(const std::weak_ptr<LUNATexture>& texture, int x, int y, int width, int height) :
	LUNAAsset(LUNAAssetType::TEXTURE_REGION), texture(texture)
{
	auto sharedTexture = texture.lock();

	// Convert pixel coordinates to UV-coordinates
	u1 = x / (float)sharedTexture->GetWidth();
	v1 = y / (float)sharedTexture->GetHeight();
	u2 = (x + width) / (float)sharedTexture->GetWidth();
	v2 = (y + height) / (float)sharedTexture->GetHeight();
}

std::weak_ptr<LUNATexture> LUNATextureRegion::GetTexture()
{
	return texture;
}

float LUNATextureRegion::GetU1()
{
	return u1;
}

float LUNATextureRegion::GetV1()
{
	return v1;
}

float LUNATextureRegion::GetU2()
{
	return u2;
}

float LUNATextureRegion::GetV2()
{
	return v2;
}

float LUNATextureRegion::GetWidth()
{
	if(texture.expired()) return 0;
	return (u2 - u1) * texture.lock()->GetWidth();
}

float LUNATextureRegion::GetHeight()
{
	if(texture.expired()) return 0;
	return (v2 - v1) * texture.lock()->GetHeight();
}
