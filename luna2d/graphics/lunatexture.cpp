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

#include "lunatexture.h"
#include "lunasizes.h"
#include "lunalog.h"

using namespace luna2d;

LUNATexture::LUNATexture(const LUNAImage& image) :
	width(image.GetWidth()),
	height(image.GetHeight()),
	colorType(image.GetColorType())
{
	CreateGlTexture(image.GetData());
}

LUNATexture::~LUNATexture()
{
	glDeleteTextures(1, &id);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Remove texture from reloadable assets list
	if(!reloadPath.empty()) LUNAEngine::SharedAssets()->SetAssetReloadable(this, false);
#endif
}

void LUNATexture::CreateGlTexture(const std::vector<unsigned char>& data)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint glColorType = GL_RGB;
	switch(colorType)
	{
	case LUNAColorType::RGBA:
		glColorType = GL_RGBA;
		break;
	case LUNAColorType::RGB:
		glColorType = GL_RGB;
		break;
	case LUNAColorType::ALPHA:
		glColorType = GL_ALPHA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glColorType, width, height, 0, glColorType, GL_UNSIGNED_BYTE, &data[0]);

	id = textureId;

	glBindTexture(GL_TEXTURE_2D, 0);
}

// Get sizes in pixels
int LUNATexture::GetWidth()
{
	return width;
}

int LUNATexture::GetHeight()
{
	return height;
}

// Get sizes in game points (scaled to virtual resoultion)
float LUNATexture::GetWidthPoints()
{
	return std::floor(width * LUNAEngine::SharedSizes()->GetTextureScale());
}

float LUNATexture::GetHeightPoints()
{
	return std::floor(height * LUNAEngine::SharedSizes()->GetTextureScale());
}

GLuint LUNATexture::GetId()
{
	return id;
}

// Check for texture is valid. Can be invalid after lost GL context
bool LUNATexture::IsValid()
{
	return glIsTexture(id) == GL_TRUE;
}

void LUNATexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void LUNATexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
