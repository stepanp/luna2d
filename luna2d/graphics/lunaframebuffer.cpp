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

#include "lunaframebuffer.h"
#include "lunagraphics.h"
#include "lunamath.h"

using namespace luna2d;

LUNAFrameBuffer::LUNAFrameBuffer(int viewportWidth, int viewportHeight) :
	viewportWidth(viewportWidth),
	viewportHeight(viewportHeight),
	texture(std::make_shared<LUNATexture>(math::NearestPowerOfTwo(viewportWidth), math::NearestPowerOfTwo(viewportHeight),
		LUNAColorType::RGB))
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetId(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) LUNA_LOGE("Failed to create GL framebuffer");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

LUNAFrameBuffer::~LUNAFrameBuffer()
{
	if(!id) return;

	Unbind();
	glDeleteFramebuffers(1, &id);
}

int LUNAFrameBuffer::GetViewportWidth()
{
	return viewportWidth;
}

int LUNAFrameBuffer::GetViewportHeight()
{
	return viewportHeight;
}

std::shared_ptr<LUNATexture> LUNAFrameBuffer::GetTexture()
{
	return texture;
}

std::shared_ptr<LUNATextureRegion> LUNAFrameBuffer::GetTextureRegion()
{
	// Texture region is flipped vertically
	float u1 = 0;
	float v1 = (float)viewportHeight / (float)texture->GetHeight();
	float u2 = (float)viewportWidth / (float)texture->GetWidth();
	float v2 = 0;

	return std::make_shared<LUNATextureRegion>(texture, u1, v1, u2, v2);
}

std::shared_ptr<LUNAImage> LUNAFrameBuffer::GetPixels()
{
	auto pixmap = texture->GetPixels();

	pixmap->SetSize(viewportWidth, viewportHeight);
	pixmap->FlipVertically();

	return pixmap;
}

void LUNAFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void LUNAFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	LUNAEngine::SharedGraphics()->GetRenderer()->SetDefaultViewport();
}
