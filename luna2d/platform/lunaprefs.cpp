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

#include "lunaprefs.h"

using namespace luna2d;

// Get type for preference
LUNAPrefType LUNAPrefs::GetPrefType(const std::string &name)
{
	std::string key = name + "_type";
	if(!HasValue(key)) return LUNAPrefType::NONE;
	return static_cast<LUNAPrefType>(GetInt(name + "_type"));
}

// Set type for preference
void LUNAPrefs::SetPrefType(const std::string &name, LUNAPrefType type)
{
	SetInt(name + "_type", static_cast<int>(type));
}

// Remove type for preference
void LUNAPrefs::RemovePrefType(const std::string &name)
{
	RemoveValue(name + "_type");
}
