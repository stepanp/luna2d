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

#include "lunasprite.h"
#include "lunaengine.h"
#include "lunagraphics.h"
#include "lunaassets.h"
#include "lunalog.h"
#include "lunamath.h"

using namespace luna2d;

LUNASprite::LUNASprite() {}

// Lua constructor
LUNASprite::LUNASprite(const LuaAny& asset)
{
	// Create sprite from texture
	texture = asset.To<std::weak_ptr<LUNATexture>>();
	if(InitFromTexture(texture)) return;

	// Create sprite from texture region
	auto region = asset.To<std::weak_ptr<LUNATextureRegion>>();
	if(InitFromRegion(region)) return;

	LUNA_LOGE("Attempt to create sprite from invalid asset");
}

LUNASprite::LUNASprite(const std::weak_ptr<LUNATexture>& texture)
{
	if(!InitFromTexture(texture)) LUNA_LOGE("Attempt to create sprite from invalid texture");
}

LUNASprite::LUNASprite(const std::weak_ptr<LUNATextureRegion>& region)
{
	if(!InitFromRegion(region)) LUNA_LOGE("Attempt to create sprite from invalid texture region");
}

bool LUNASprite::InitFromTexture(const std::weak_ptr<LUNATexture>& texture)
{
	if(texture.expired()) return false;

	u1 = 0;
	v1 = 0;
	u2 = 1;
	v2 = 1;

	// Use texture size in points (scaled to virtual resolution)
	auto sharedTexture = texture.lock();
	width = sharedTexture->GetWidthPoints();
	height = sharedTexture->GetHeightPoints();

	return true;
}

bool LUNASprite::InitFromRegion(const std::weak_ptr<LUNATextureRegion>& region)
{
	if(region.expired()) return false;

	auto sharedRegion = region.lock();
	texture = sharedRegion->GetTexture();
	if(texture.expired()) return false;

	u1 = sharedRegion->GetU1();
	v1 = sharedRegion->GetV1();
	u2 = sharedRegion->GetU2();
	v2 = sharedRegion->GetV2();

	// Convert sizes to virtual resolution
	width = sharedRegion->GetWidthPoints();
	height = sharedRegion->GetHeightPoints();
	return true;
}

void LUNASprite::SetTexture(const std::weak_ptr<LUNATexture>& texture)
{
	if(texture.expired()) LUNA_RETURN_ERR("Attempt set invalid texure to sprite");

	this->texture = texture.lock();
	u1 = 0;
	v1 = 0;
	u2 = 1;
	v2 = 1;
}

void LUNASprite::SetTextureRegion(const std::weak_ptr<LUNATextureRegion>& region)
{
	if(!region.expired())
	{
		auto sharedRegion = region.lock();
		auto regionTexture = sharedRegion->GetTexture();

		if(!regionTexture.expired())
		{
			texture = regionTexture.lock();
			u1 = sharedRegion->GetU1();
			v1 = sharedRegion->GetV1();
			u2 = sharedRegion->GetU2();
			v2 = sharedRegion->GetV2();
			return;
		}
	}

	LUNA_RETURN_ERR("Attempt set invalid texure region to sprite")
}

float LUNASprite::GetX()
{
	return x;
}

float LUNASprite::GetY()
{
	return y;
}

void LUNASprite::SetX(float x)
{
	this->x = x;
}

void LUNASprite::SetY(float y)
{
	this->y = y;
}

glm::vec2 LUNASprite::GetPos()
{
	return glm::vec2(x, y);
}

void LUNASprite::SetPos(float x, float y)
{
	SetX(x);
	SetY(y);
}

float LUNASprite::GetWidth()
{
	return width;
}

float LUNASprite::GetHeight()
{
	return height;
}

void LUNASprite::SetWidth(float width)
{
	this->width = width;
}

void LUNASprite::SetHeight(float height)
{
	this->height = height;
}

void LUNASprite::SetSize(float width, float height)
{
	SetWidth(width);
	SetHeight(height);
}

float LUNASprite::GetOriginX()
{
	return originX;
}

float LUNASprite::GetOriginY()
{
	return originY;
}

void LUNASprite::SetOriginX(float originX)
{
	this->originX = originX;
}

void LUNASprite::SetOriginY(float originY)
{
	this->originY = originY;
}

void LUNASprite::SetOrigin(float originX, float originY)
{
	SetOriginX(originX);
	SetOriginY(originY);
}

// Set origin to center of sprite
void LUNASprite::SetOriginToCenter()
{
	SetOrigin(width / 2, height / 2);
}

float LUNASprite::GetScaleX()
{
	return scaleX;
}

float LUNASprite::GetScaleY()
{
	return scaleY;
}

void LUNASprite::SetScaleX(float scaleX)
{
	this->scaleX = scaleX;
}

void LUNASprite::SetScaleY(float scaleY)
{
	this->scaleY = scaleY;
}

void LUNASprite::SetScale(float scale)
{
	SetScaleX(scale);
	SetScaleY(scale);
}

void LUNASprite::Render()
{
	if(texture.expired())
	{
		LUNA_LOGE("Attempt to render invalid sprite");
		return;
	}

	LUNARenderer* renderer = LUNAEngine::SharedGraphics()->GetRenderer();

	// Sprite geometry
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = height * scaleY;
	float x3 = width * scaleX;
	float y3 = height * scaleY;
	float x4 = width * scaleX;
	float y4 = 0;

	// Offset by origin
	float offsetX = originX * scaleX;
	float offsetY = originY * scaleY;
	x1 -= offsetX;
	y1 -= offsetY;
	x2 -= offsetX;
	y2 -= offsetY;
	x3 -= offsetX;
	y3 -= offsetY;
	x4 -= offsetX;
	y4 -= offsetY;

	// Rotate
	if(angle != 0)
	{
		float sin = std::sin(math::DegreesToRadians(angle));
		float cos = std::cos(math::DegreesToRadians(angle));

		float rx1 = x1 * cos - y1 * sin;
		float ry1 = x1 * sin + y1 * cos;
		float rx2 = x2 * cos - y2 * sin;
		float ry2 = x2 * sin + y2 * cos;
		float rx3 = x3 * cos - y3 * sin;
		float ry3 = x3 * sin + y3 * cos;
		float rx4 = x4 * cos - y4 * sin;
		float ry4 = x4 * sin + y4 * cos;

		x1 = rx1;
		y1 = ry1;
		x2 = rx2;
		y2 = ry2;
		x3 = rx3;
		y3 = ry3;
		x4 = rx4;
		y4 = ry4;
	}

	renderer->RenderQuad(texture.lock().get(),
		x + x1, y + y1, u1, v2, x + x2, y + y2, u1, v1, x + x3, y + y3, u2, v1, x + x4, y + y4, u2, v2, color);
}

// Get rotation angle (in degrees)
float LUNASprite::GetAngle()
{
	return angle;
}

// Set rotation angle (in degrees)
void LUNASprite::SetAngle(float angle)
{
	this->angle = angle;
}

void LUNASprite::SetColor(float r, float g, float b)
{
	color.r = r / 255.0f;
	color.g = g / 255.0f;
	color.b = b / 255.0f;
}

LUNAColor LUNASprite::GetColor()
{
	return color;
}

void LUNASprite::SetAlpha(float alpha)
{
	color.a = alpha;
}

float LUNASprite::GetAlpha()
{
	return color.a;
}
