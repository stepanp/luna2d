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

#include "lunajsonutils.h"

using namespace luna2d;
using namespace json11;

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
LuaTable luna2d::Json2Lua(const json11::Json& json)
{
	if(json.is_array()) return JsonArray2Lua(json.array_items());
	else if(json.is_object()) return JsonObject2Lua(json.object_items());

	return LuaTable();
}

// Serialize lua table to json array
Json luna2d::Lua2JsonArray(const LuaTable& table)
{
	Json::array ret;

	auto luaVm = table.GetRef()->GetLuaVm();

	LuaStack<LuaTable>::Push(luaVm, table);
	if(!lua_istable(luaVm, -1))
	{
		lua_pop(luaVm, 1);
		return std::move(ret);
	}

	int count = lua_rawlen(luaVm, -1);
	ret.reserve(count);

	for(int i = 0; i < count; i++)
	{
		lua_rawgeti(luaVm, -1, i + 1);

		switch(lua_type(luaVm, -1))
		{
		case LUA_TNIL:
			ret.push_back(std::nullptr_t());
			break;
		case LUA_TNUMBER:
			ret.push_back(LuaStack<float>::Pop(luaVm, -1));
			break;
		case LUA_TBOOLEAN:
			ret.push_back(LuaStack<bool>::Pop(luaVm, -1));
			break;
		case LUA_TSTRING:
			ret.push_back(LuaStack<std::string>::Pop(luaVm, -1));
			break;
		case LUA_TTABLE:
			ret.push_back(Lua2Json(LuaStack<LuaTable>::Pop(luaVm, -1)));
			break;
		}

		lua_pop(luaVm, 1);
	}

	lua_pop(luaVm, 1);

	return std::move(ret);
}

// Serialize lua table to json object
Json luna2d::Lua2JsonObject(const LuaTable& table)
{
	Json::object ret;

	for(const auto& entry : table)
	{
		const auto& key = entry.first.ToString();
		const auto& value = entry.second;

		switch(value.GetType())
		{
		case LUA_TNIL:
			ret[key] = std::nullptr_t();
			break;
		case LUA_TNUMBER:
			ret[key] = value.ToFloat();
			break;
		case LUA_TBOOLEAN:
			ret[key] = value.ToBool();
			break;
		case LUA_TSTRING:
			ret[key] = value.ToString();
			break;
		case LUA_TTABLE:
			ret[key] = Lua2Json(value.ToTable());
			break;
		}
	}

	return std::move(ret);
}

// Serialize table to json object
Json luna2d::Lua2Json(const LuaTable& table)
{
	if(table.GetArrayCount() > 0) return Lua2JsonArray(table);
	else return Lua2JsonObject(table);
}
