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

#include "lunastrings.h"
#include "lunafiles.h"
#include <json11.hpp>

using namespace luna2d;
using namespace json11;

LUNAStrings::LUNAStrings():
	tblStrings(LUNAEngine::SharedLua())
{
	FetchLocales();
	if(localesList.empty()) return;

	if(defaultLocale.empty()) LUNA_LOGW("Default locale not found");

	// Automatically select current locale
	// Locale selects with following priority:
	//  1. System locale
	//  2. System language (Ignore country)
	//  3. Default locale
	curLocale = GetSystemLocale();
	if(!HasLocale(curLocale)) curLocale = ParseLang(curLocale);
	if(!HasLocale(curLocale)) curLocale = defaultLocale;

	LoadStrings();

	LuaScript* lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	// Bind strings table to lua
	tblLuna.SetField("strings", tblStrings);
	tblStrings.MakeReadOnly();

	// Bind strings manager to lua
	LuaTable tblStringsMgr(lua);
	tblStringsMgr.SetField("getLocale", LuaFunction(lua, this, &LUNAStrings::GetLocale));
	tblStringsMgr.SetField("getDefaultLocale", LuaFunction(lua, this, &LUNAStrings::GetDefaultLocale));
	tblStringsMgr.SetField("getSystemLocale", LuaFunction(lua, this, &LUNAStrings::GetSystemLocale));
	tblStringsMgr.SetField("setLocale", LuaFunction(lua, this, &LUNAStrings::SetLocale));
	tblStringsMgr.SetField("hasLocale", LuaFunction(lua, this, &LUNAStrings::HasLocale));
	tblStringsMgr.SetField("getLocalesList", LuaFunction(lua, this, &LUNAStrings::GetLocalesList));
	tblStringsMgr.SetField("parseLang", LuaFunction(lua, this, &LUNAStrings::ParseLang));
	tblStringsMgr.SetField("parseCountry", LuaFunction(lua, this, &LUNAStrings::ParseLang));
	tblStringsMgr.SetField("isValidLocale", LuaFunction(lua, this, &LUNAStrings::IsValidLocale));
	tblLuna.SetField("stringsmgr", tblStringsMgr);
}

// Fetch all available locales
void LUNAStrings::FetchLocales()
{
	LUNAFiles* files = LUNAEngine::SharedFiles();
	auto localeFiles = files->GetFileList(LOCALIZATION_PATH, LUNAFileLocation::ASSETS);

	for(auto& filename : localeFiles)
	{
		if(files->GetExtension(filename) != LOCALE_FILE_EXTENSION) continue;

		auto locale = files->SplitResolutionSuffix(files->GetBasename(filename));
		if(IsValidLocale(locale.first))
		{
			localesList.insert(locale.first);

			// Select default locale
			if(defaultLocale.empty() && locale.second == DEFAULT_LOCALE_SUFFIX) SetDefaultLocale(locale.first);
		}
		else LUNA_LOGE("Invalid locale \"%s\"", locale.first.c_str());
	}
}

// Load strings for current locale
void LUNAStrings::LoadStrings()
{
	tblStrings.Clear();

	if(!HasLocale(curLocale)) LUNA_RETURN_ERR("Strings for locale \"%s\" not found", curLocale.c_str());

	std::string locale = curLocale;
	if(locale == defaultLocale) locale += "@" + DEFAULT_LOCALE_SUFFIX;
	std::string filename = LOCALIZATION_PATH + locale + "." + LOCALE_FILE_EXTENSION;

	std::string localeData = LUNAEngine::SharedFiles()->ReadFileToString(filename, LUNAFileLocation::ASSETS);
	if(localeData.empty()) LUNA_RETURN_ERR("Cannot load strings form file \"%s\"", filename.c_str());

	std::string err;
	Json jsonLocale = Json::parse(localeData, err);
	if(jsonLocale == nullptr) LUNA_RETURN_ERR("Cannot load strings: %s", err.c_str());

	for(auto entry : jsonLocale.object_items())
	{
		const std::string& name = entry.first;
		const std::string& string  = entry.second.string_value();

		tblStrings.SetField(name, string, true);
	}
}

std::string LUNAStrings::GetString(const std::string& name)
{
	auto ret = tblStrings.GetString(name);
	if(ret.empty()) return name;
	return std::move(ret);
}

std::string LUNAStrings::GetLocale()
{
	return curLocale;
}

std::string LUNAStrings::GetDefaultLocale()
{
	return defaultLocale;
}

std::string LUNAStrings::GetSystemLocale()
{
	return "ru_RU";
}

void LUNAStrings::SetLocale(const std::string& locale)
{
	curLocale = locale;
	LoadStrings();
}

void LUNAStrings::SetDefaultLocale(const std::string& locale)
{
	defaultLocale = locale;
}

bool LUNAStrings::HasLocale(const std::string& locale)
{
	return localesList.count(locale) == 1;
}

const std::unordered_set<std::string>& LUNAStrings::GetLocalesList()
{
	return localesList;
}

// Parse language from full locale
std::string LUNAStrings::ParseLang(const std::string& locale)
{
	if(!IsValidLocale(locale)) return "";
	return locale.substr(0, 2);
}

// Parse country from full locale
std::string LUNAStrings::ParseCountry(const std::string& locale)
{
	if(!IsValidLocale(locale)) return "";
	return locale.substr(3, 2);
}

// Check if given string is valid locale name
bool LUNAStrings::IsValidLocale(const std::string& locale)
{
	size_t len = locale.length();

	if(len != 2 && len != 5) return false;
	if(len == 5 && locale[2] != '_' && locale[2] != '-') return false;
	if(!std::islower(locale[0]) || !std::islower(locale[1])) return false;
	if(len == 5 && (!std::isupper(locale[3]) || !std::isupper(locale[4]))) return false;

	return true;
}
