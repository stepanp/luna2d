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

#include "lunawstring.h"

using namespace luna2d;

// Convert std::string to std::wstring
std::wstring luna2d::ToWString(const std::string& str)
{
	std::wstring ret(str.length(), L'\n');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &ret[0], ret.length());

	return std::move(ret);
}

// Convert std::wstring to std::string
std::string luna2d::FromWString(const std::wstring& str)
{
	std::string ret(str.length(), '\n');
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.length(), &ret[0], ret.length(), "", FALSE);

	return std::move(ret);
}

// Convert std::string to Platform::String
Platform::String^ luna2d::ToPlatformString(const std::string& str)
{
	std::wstring wstr = std::wstring(str.begin(), str.end());
	return ref new Platform::String(wstr.c_str());
}

// Convert Platform::String to std::string
std::string luna2d::FromPlatfromString(Platform::String^ str)
{
	std::wstring wstr = str->Data();
	return std::string(wstr.begin(), wstr.end());
}