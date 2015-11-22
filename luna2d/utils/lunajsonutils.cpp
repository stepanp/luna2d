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

#include "lunajsonutils.h"

using namespace luna2d;
using namespace json11;

enum class StringState
{
	NONE,
	SINGLE_LINE,
	MULTI_LINE,
	STRING,
};

// Parse json to strign and strip comments
std::string luna2d::StripJsonComments(const std::string& data)
{
	StringState state = StringState::NONE;
	std::string ret;
	ret.reserve(data.size());

	size_t i = 0;
	while(i < data.length())
	{
		if(state == StringState::NONE)
		{
			// Remark about "data[i + 1]" and "while(i < data.length())"
			// It's safety usage, because since C++11 internal buffer of std::string must ends with zero char

			if(data[i] == '/' && data[i + 1] == '/') state = StringState::SINGLE_LINE;
			else if(data[i] == '/' && data[i + 1] == '*') state = StringState::MULTI_LINE;
			else if(data[i] == '"')
			{
				state = StringState::STRING;
				ret += data[i];
			}
			else ret += data[i];

			i++;
		}

		else if(state == StringState::SINGLE_LINE)
		{
			if(data[i] == '\n') state = StringState::NONE;
			else i++;
		}

		else if(state == StringState::MULTI_LINE)
		{
			if(data[i] == '*' && data[i + 1] == '/')
			{
				state = StringState::NONE;
				i += 2;
			}
			else i++;
		}

		else if(state == StringState::STRING)
		{
			if(data[i] == '"') state = StringState::NONE;

			ret += data[i];
			i++;
		}
	}

	return std::move(ret);
}

// Deserialize json array to lua table
LuaTable luna2d::JsonArray2Lua(const Json::array& array)
{
	LuaTable ret(LUNAEngine::SharedLua());

	size_t i = 1;
	for(const auto& value : array)
	{
		switch(value.type())
		{
		case Json::NUL:
			ret.SetArrayField(i, nil);
			break;
		case Json::NUMBER:
			ret.SetArrayField(i, static_cast<float>(value.number_value()));
			break;
		case Json::BOOL:
			ret.SetArrayField(i, value.bool_value());
			break;
		case Json::STRING:
			ret.SetArrayField(i, value.string_value());
			break;
		case Json::ARRAY:
			ret.SetArrayField(i, JsonArray2Lua(value.array_items()));
			break;
		case Json::OBJECT:
			ret.SetArrayField(i, JsonObject2Lua(value.object_items()));
			break;
		}

		i++;
	}

	return ret;
}

// Deserialize json object to lua table
LuaTable luna2d::JsonObject2Lua(const Json::object& object)
{
	LuaTable ret(LUNAEngine::SharedLua());

	for(const auto& entry : object)
	{
		const std::string& key = entry.first;
		const Json& value = entry.second;

		switch(value.type())
		{
		case Json::NUL:
			ret.SetField(key, nil);
			break;
		case Json::NUMBER:
			ret.SetField(key, static_cast<float>(value.number_value()));
			break;
		case Json::BOOL:
			ret.SetField(key, value.bool_value());
			break;
		case Json::STRING:
			ret.SetField(key, value.string_value());
			break;
		case Json::ARRAY:
			ret.SetField(key, JsonArray2Lua(value.array_items()));
			break;
		case Json::OBJECT:
			ret.SetField(key, JsonObject2Lua(value.object_items()));
			break;
		}
	}

	return ret;
}

// Deserialize root json object to lua table
LuaTable luna2d::Json2Lua(json11::Json json)
{
	if(json.is_array()) return JsonArray2Lua(json.array_items());
	else if(json.is_object()) return JsonObject2Lua(json.object_items());

	return LuaTable();
}
