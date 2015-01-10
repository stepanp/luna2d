//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#include "lunafiles.h"

using namespace luna2d;

// Get extension of file
std::string LUNAFiles::GetExtension(const std::string& path)
{
	size_t pos = path.rfind('.');
	if(pos == std::string::npos) return ""; // File hasn't extension

	return std::move(path.substr(pos + 1));
}

// Get filename without path and extension
std::string LUNAFiles::GetBasename(const std::string& path)
{
	size_t extPos = path.rfind('.'); // Extension pos
	size_t slashPos = path.rfind('/'); // Last slash pos

	if(extPos == std::string::npos) extPos = path.length();

	return std::move(path.substr(slashPos + 1, extPos - slashPos - 1));
}

// Get directory part of path
std::string LUNAFiles::GetParentPath(const std::string& path)
{
	size_t slashPos = path.rfind('/'); // Last slash pos

	if(slashPos == std::string::npos) return std::move("/");
	return path.substr(0, path.rfind('/'));
}

// Split basename to name and resolution suffix
// Return pair: name - suffix
std::pair<std::string,std::string> LUNAFiles::SplitResolutionSuffix(const std::string &basename)
{
	size_t suffixPos = basename.rfind("@");
	if(suffixPos == std::string::npos) return std::make_pair(basename, "");
	else
	{
		std::string name = basename.substr(0, suffixPos);
		std::string suffix = basename.substr(suffixPos + 1);
		return std::make_pair(std::move(name), std::move(suffix));
	}
}
