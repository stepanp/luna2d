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
	mesh.SetShader(LUNAEngine::SharedGraphics()->GetRenderer()->GetFontShader());
}

void LUNAText::Build()
{
	mesh.Clear();

	auto sharedFont = font.lock();
	if(!sharedFont) LUNA_RETURN_ERR("Attemp to render invalid text object");

	float pointerX = 0;
	height = 0;

	for(char32_t c : this->text)
	{
		const auto& glyph = sharedFont->GetGlyphForChar(c);
		const auto& region = glyph.region;

		mesh.AddQuad(x + pointerX + glyph.offsetX * scaleX, y + glyph.offsetY * scaleY,
			region->GetWidthPoints() * scaleX,
			region->GetHeightPoints() * scaleY,
			region->GetU1(),
			region->GetV1(),
			region->GetU2(),
			region->GetV2(),
			color, color.a);

		pointerX += glyph.width * scaleX;
		height = std::max(height, glyph.height * scaleY);
	}

	width = pointerX;

	dirty = false;
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
	SetPos(x, this->y);
}

void LUNAText::SetY(float y)
{
	SetPos(this->x, y);
}

glm::vec2 LUNAText::GetPos()
{
	return glm::vec2(x, y);
}

void LUNAText::SetPos(float x, float y)
{
	this->x = x;
	this->y = y;
	dirty = true;
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
	dirty = true;
}

void LUNAText::SetScaleY(float scaleY)
{
	this->scaleY = scaleY;
	dirty = true;
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
	dirty = true;
}

LUNAColor LUNAText::GetColor()
{
	return color;
}

void LUNAText::SetAlpha(float alpha)
{
	color.a = alpha;
	dirty = true;
}

float LUNAText::GetAlpha()
{
	return color.a;
}

void LUNAText::SetFont(const std::weak_ptr<LUNAFont>& font)
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to set invalid font to text object");
		return;
	}

	this->font = font;
	mesh.SetTexture(font.lock()->GetTexture());
	dirty = true;
}

void LUNAText::SetShader(const std::weak_ptr<LUNAShader>& shader)
{
	mesh.SetShader(shader);
}

float LUNAText::GetWidth()
{
	return width;
}

float LUNAText::GetHeight()
{
	return height;
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

	Build();
}

void LUNAText::Render()
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to render invalid text object");
		return;
	}

	if(text.empty()) return;

	if(dirty) Build();

	mesh.Render();
}
