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

#include "lunamaterial.h"
#include "lunagraphics.h"

using namespace luna2d;

LUNAMaterial::LUNAMaterial() :
	shader(LUNAEngine::SharedGraphics()->GetRenderer()->GetDefaultShader()),
	blending(LUNABlendingMode::ALPHA)
{
}

LUNAMaterial::LUNAMaterial(const std::weak_ptr<LUNATexture>& texture, const std::weak_ptr<LUNAShader>& shader,
	LUNABlendingMode blending) :
	texture(texture),
	shader(shader),
	blending(blending)
{
}

bool LUNAMaterial::operator==(const LUNAMaterial& material) const
{
	if(blending != material.blending) return false;
	if(texture.lock() != material.texture.lock()) return false;
	if(shader.lock() != material.shader.lock()) return false;

	return true;
}

bool LUNAMaterial::operator!=(const LUNAMaterial& material) const
{
	return !operator==(material);
}
