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

#pragma once

#include "lunaengine.h"

namespace luna2d{

template<typename EnumType>
class LUNAStringEnum
{
public:
	LUNAStringEnum(const std::initializer_list<std::string>& keys)
	{
		size_t index = 0;
		for(const auto& key : keys)
		{
			map[key] = static_cast<EnumType>(index);
			index++;
		}
	}

private:
	std::unordered_map<std::string,EnumType> map;

public:
	bool HasKey(const std::string& key) const
	{
		return map.count(key) == 1;
	}

	EnumType FromString(const std::string& key) const
	{
		auto it = map.find(key);
		if(it == map.end()) return static_cast<EnumType>(-1);

		return it->second;
	}

	EnumType FromString(const std::string& key, EnumType defaultValue) const
	{
		auto it = map.find(key);
		if(it == map.end()) return defaultValue;

		return it->second;
	}

	std::string FromEnum(EnumType value) const
	{
		for(const auto& entry : map)
		{
			if(entry.second == value) return entry.first;
		}

		return "";
	}
};

}
