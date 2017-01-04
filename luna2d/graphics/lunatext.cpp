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

#include "lunatext.h"
#include "lunautf.h"
#include "lunagraphics.h"

using namespace luna2d;

LUNAText::LUNAText(const std::weak_ptr<LUNAFont>& font)
{
	SetFont(font);
}

float LUNAText::GetX()
{
	return x;
}

float LUNAText::GetY()
{
	return y;
}

void LUNAText::SetX(float x)
{
	this->x = x;
}

void LUNAText::SetY(float y)
{
	this->y = y;
}

glm::vec2 LUNAText::GetPos()
{
	return glm::vec2(x, y);
}

void LUNAText::SetPos(float x, float y)
{
	this->x = x;
	this->y = y;
}

float LUNAText::GetScaleX()
{
	return scaleX;
}

float LUNAText::GetScaleY()
{
	return scaleY;
}

void LUNAText::SetScaleX(float scaleX)
{
	this->scaleX = scaleX;
}

void LUNAText::SetScaleY(float scaleY)
{
	this->scaleY = scaleY;
}

void LUNAText::SetScale(float scale)
{
	SetScaleX(scale);
	SetScaleY(scale);
}


void LUNAText::SetColor(float r, float g, float b)
{
	color.r = r / 255.0f;
	color.g = g / 255.0f;
	color.b = b / 255.0f;
}

LUNAColor LUNAText::GetColor()
{
	return color;
}

void LUNAText::SetAlpha(float alpha)
{
	color.a = alpha;
}

float LUNAText::GetAlpha()
{
	return color.a;
}

void LUNAText::SetFont(const std::weak_ptr<LUNAFont> font)
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to set invalid font to text object");
		return;
	}

	this->font = font;
}

float LUNAText::GetWidth()
{
	if(sprites.empty()) return 0.0f;

	float width = 0;
	for(auto& spr : sprites) width += spr->GetWidth();

	return width;
}

float LUNAText::GetHeight()
{
	if(sprites.empty()) return 0.0f;

	float maxHeight = 0.0f;
	for(auto& spr : sprites) maxHeight = std::max(maxHeight, spr->GetHeight());

	return maxHeight;
}

// Get text value in UTF-8 encoding
std::string LUNAText::GetText()
{
	// Convert saved string from UTF-32 to UTF-8
	return utf::FromUtf32(text);
}

// Set text value. Given text in UTF-8 encoding
void LUNAText::SetText(const std::string& text)
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to set text value to invalid text object");
		return;
	}

	// Convert given string from UTF-8 to UTF-32
	this->text = utf::ToUtf32(text);

	auto sharedFont = font.lock();
	sprites.erase(sprites.begin(), sprites.end());
	sprites.clear();
	for(char32_t c : this->text)
	{
		auto sprite = std::make_shared<LUNASprite>(sharedFont->GetRegionForChar(c));
		sprite->SetShader(LUNAEngine::SharedGraphics()->GetRenderer()->GetFontShader());
		sprites.push_back(sprite);
	}
}

void LUNAText::Render()
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to render invalid text object");
		return;
	}

	int offset = 0;
	for(auto& spr : sprites)
	{
		spr->SetPos(x + offset, y);
		spr->SetScaleX(scaleX);
		spr->SetScaleY(scaleY);
		spr->SetColor(color.GetR(), color.GetG(), color.GetB());
		spr->SetAlpha(color.a);
		spr->Render();

		offset += std::roundf(spr->GetWidth() * spr->GetScaleX());
	}
}
