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

#include "lunatext.h"

using namespace luna2d;

LUNAText::LUNAText(const std::weak_ptr<LUNAFont>& font)
{
	SetFont(font);
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

void LUNAText::SetText(const std::string& text)
{
	if(font.expired())
	{
		LUNA_LOGE("Attemp to set text value to invalid text object");
		return;
	}

	this->text = text;

	auto sharedFont = font.lock();
	sprites.clear();
	for(char c : text)
	{
		auto sprite = std::make_shared<LUNASprite>(sharedFont->GetRegionForChar(c));
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
	for(auto sprite : sprites)
	{
		sprite->SetX(offset);
		sprite->Render();
		offset += sprite->GetWidth();
	}
}
