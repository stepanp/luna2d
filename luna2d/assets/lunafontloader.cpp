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

#include "lunafontloader.h"
#include "lunafontgenerator.h"
#include "lunafiles.h"
#include "lunajsonutils.h"

using namespace luna2d;
using namespace json11;

bool LUNAFontLoader::Load(const std::string& filename)
{
	LUNAFiles* files = LUNAEngine::SharedFiles();

	// Load description file for font
	std::string desc = files->ReplaceExtension(filename, "font");
	if(!files->IsFile(desc)) return false;

	std::string descData = files->ReadFileToString(desc);
	std::string err;
	Json jsonDesc = Json::parse(StripJsonComments(descData), err);
	if(jsonDesc == nullptr)
	{
		LUNA_LOGE(err.c_str());
		return false;
	}

	auto jsonSizes = jsonDesc["sizes"];
	if(!jsonSizes.is_object())
	{
		LUNA_LOGE("Invalid font description file");
		return false;
	}

	// Load font file
	LUNAFontGenerator generator;
	if(!generator.Load(filename)) return false;

	// Enable\disable characters sets
	auto jsonChars = jsonDesc["chars"];
	if(jsonChars.is_object())
	{
		generator.enableLatin = jsonChars["latin"].bool_value() == true;
		generator.enableCyrillic = jsonChars["cyrillic"].bool_value() == true;
		generator.enableCommon = jsonChars["common"].bool_value() == true;
		generator.enableNumbers = jsonChars["numbers"].bool_value() == true;
	}

	// Generate bitmap fonts for each specifed size in description file
	for(auto entry : jsonSizes.object_items())
	{
		fonts[entry.first] = generator.GenerateFont(entry.second.int_value());
	}

	return !fonts.empty();
}

void LUNAFontLoader::PushToLua(const std::string& name, luna2d::LuaTable& parentTable)
{
	LuaTable tblFont(LUNAEngine::SharedLua());
	tblFont.MakeReadOnly();
	parentTable.SetField(name, tblFont, true);

	for(auto entry : fonts) tblFont.SetField(entry.first, entry.second, true);
}
