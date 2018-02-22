//-----------------------------------------------------------------------------
// luna2d Emulator
// This is part of luna2d engine
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

#pragma once

#include <QString>

enum class ScreenOrientation
{
	PORTRAIT,
	LANDSCAPE
};

struct ScreenMargins
{
	ScreenMargins(int top, int bottom) :
		top(top), bottom(bottom) {}

	ScreenMargins() {}

	int top = 0;
	int bottom = 0;
};

struct Resolution
{
	Resolution(int width, int height, const QString& name, bool bullitin = false) :
		width(width), height(height), name(name), bullitin(bullitin) {}

	Resolution(int width, int height, const QString& name,
		const ScreenMargins& portrait, const ScreenMargins& landscape, bool bullitin = false) :
		width(width), height(height), name(name),
		portraitMargins(portraitMargins), landscapeMargins(landscapeMargins), bullitin(bullitin) {}

	Resolution() {}

	int width, height;
	QString name;
	ScreenMargins portraitMargins;
	ScreenMargins landscapeMargins;
	bool bullitin;
};
