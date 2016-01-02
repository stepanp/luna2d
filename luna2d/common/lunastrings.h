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

#include "lunaengine.h"
#include "lunalua.h"

namespace luna2d{

const std::string DEFAULT_LOCALE_SUFFIX = "Default";
const std::string LOCALE_FILE_EXTENSION = "json";

//-----------------------
// Localization subsystem
//-----------------------
class LUNAStrings
{
public:
	LUNAStrings();

private:
	std::string defaultLocale;
	std::string curLocale;
	LuaTable tblStrings; // Loaded strings, binded to lua
	std::unordered_set<std::string> localesList; // List of available locales

private:
	void FetchLocales(); // Fetch all available locales
	void LoadStrings(); // Load strings for current locale

public:
	std::string GetString(const std::string& name);
	std::string GetLocale();
	std::string GetDefaultLocale();
	std::string GetSystemLocale();
	void SetLocale(const std::string& locale);
	void SetDefaultLocale(const std::string& locale);
	bool HasLocale(const std::string& locale);
	const std::unordered_set<std::string>& GetLocalesList();

	std::string ParseLang(const std::string& locale); // Parse language from full locale
	std::string ParseCountry(const std::string& locale); // Parse country from full locale
	bool IsValidLocale(const std::string& locale); // Check if given string is valid locale name
};

}
