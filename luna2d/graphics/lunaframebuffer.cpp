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
#include "lunapngformat.h"

using namespace luna2d;

LUNAFrameBuffer::LUNAFrameBuffer(int viewportWidth, int viewportHeight, LUNAColorType colorType) :
	viewportWidth(viewportWidth),
	viewportHeight(viewportHeight),
	texture(std::make_shared<LUNATexture>(math::NearestPowerOfTwo(viewportWidth), math::NearestPowerOfTwo(viewportHeight),
		colorType))
{
	GLint prevId = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevId);

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetId(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) LUNA_LOGE("Failed to create GL framebuffer");

	glBindFramebuffer(GL_FRAMEBUFFER, prevId);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Add framebuffer to reloadable assets list
	LUNAEngine::SharedAssets()->SetAssetReloadable(this, true);
#endif
}

LUNAFrameBuffer::~LUNAFrameBuffer()
{
	glDeleteFramebuffers(1, &id);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Remove framebuffer from reloadable assets list
	LUNAEngine::SharedAssets()->SetAssetReloadable(this, false);
#endif
}

GLuint LUNAFrameBuffer::GetId()
{
	return id;
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

std::shared_ptr<LUNAImage> LUNAFrameBuffer::ReadPixels()
{
	std::vector<unsigned char> data(viewportWidth * viewportHeight * GetBytesPerPixel(LUNAColorType::RGBA));

	GLint prevId = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevId);

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, viewportWidth, viewportHeight, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, prevId);
	LUNAEngine::SharedGraphics()->GetRenderer()->SetDefaultViewport();

	auto pixmap = std::make_shared<LUNAImage>(viewportWidth, viewportHeight, LUNAColorType::RGBA, data);
	pixmap->FlipVertically();
	return pixmap;
}

void LUNAFrameBuffer::Bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &this->prevId);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, viewportWidth, viewportHeight);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	needCache = true;
#endif
}

void LUNAFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->prevId);
	LUNAEngine::SharedGraphics()->GetRenderer()->SetDefaultViewport();
}


// Recreate framebuffer and reload attached texture when application lost OpenGL context
// SEE: "lunaassets.h"
#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
void LUNAFrameBuffer::Reload()
{
	// Reload texture
	texture->Reload();

	// Recreate framebuffer
	GLint prevId = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevId);

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetId(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) LUNA_LOGE("Failed to create GL framebuffer");

	glBindFramebuffer(GL_FRAMEBUFFER, prevId);
}

void LUNAFrameBuffer::Cache()
{
	if(!needCache) return;

	std::vector<unsigned char> data(texture->GetWidth() * texture->GetHeight() * GetBytesPerPixel(LUNAColorType::RGBA));

	GLint prevId = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevId);

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, texture->GetWidth(), texture->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, prevId);
	LUNAEngine::SharedGraphics()->GetRenderer()->SetDefaultViewport();

	texture->Cache(data, false);

	needCache = false;
}
#endif
