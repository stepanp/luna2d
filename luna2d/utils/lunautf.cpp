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

#include "lunautf.h"
#include <utf8.h>

using namespace luna2d;
using namespace ::utf8::unchecked; // Do not use exceptions

// Convert UTF-8 string to UTF-32 string
std::u32string luna2d::utf::ToUtf32(const std::string& string)
{
	std::u32string ret;
	utf8to32(string.begin(), string.end(), std::back_inserter(ret));
	return std::move(ret);
}

// Convert UTF-32 string to UTF-8 string
std::string luna2d::utf::FromUtf32(const std::u32string& string)
{
	std::string ret;
	utf32to8(string.begin(), string.end(), std::back_inserter(ret));
	return std::move(ret);
}
