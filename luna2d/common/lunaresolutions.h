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

#pragma once

#include <unordered_map>

namespace luna2d{

const std::string DEFAULT_RESOLUTION = "HVGA";

// List of all available screen resolutions
// For calculatring virtual resolution needs only height, so table not store resolutions width
const std::unordered_map<std::string, int> RESOLUTIONS_TABLE =
{
	{"HVGA", 320}, // 480x320
	{"WVGA", 480}, // 800x480
	{"qHD", 540}, // 960x540
	{"HD", 720}, // 1280x720
	{"FullHD", 1080}, // 1920x1080
	{"WSVGA", 600}, // 1024x600
	{"WXGA", 800}, // 1280x800
	{"Retina iPad", 1536}, // 2048x1536
	{"WQXGA", 1600}, //2560x1600
};

}
