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
	Json jsonDesc = Json::parse(descData, err, JsonParse::COMMENTS);
	if(jsonDesc == nullptr)
	{
		LUNA_LOGE(err.c_str());
		return false;
	}

	// Load font file
	LUNAFontGenerator generator;
	if(!generator.Load(filename)) return false;

	// Generate bitmap fonts for each specifed size in description file
	for(auto entry : jsonDesc.object_items())
	{
		generator.ResetCharSets();

		if(entry.second.is_number()) fonts[entry.first] = generator.GenerateFont(entry.second.int_value());
		else if(entry.second.is_object())
		{
			auto sizeParams = entry.second;

			if(!sizeParams["size"].is_number())
			{
				LUNA_LOGE("Invalid parameters for \"%s\" font size ", entry.first.c_str());
				continue;
			}

			auto jsonChars = sizeParams["chars"];
			if(jsonChars.is_object())
			{
				generator.enableLatin = jsonChars["latin"].bool_value() == true;
				generator.enableCyrillic = jsonChars["cyrillic"].bool_value() == true;
				generator.enableCommon = jsonChars["common"].bool_value() == true;
				generator.enableNumbers = jsonChars["numbers"].bool_value() == true;
			}

			fonts[entry.first] = generator.GenerateFont(sizeParams["size"].int_value());
		}
	}

	return !fonts.empty();
}

void LUNAFontLoader::PushToLua(const std::string& name, LuaTable& parentTable)
{
	LuaTable tblFont(LUNAEngine::SharedLua());
	tblFont.MakeReadOnly();
	parentTable.SetField(name, tblFont, true);

	for(auto entry : fonts) tblFont.SetField(entry.first, entry.second, true);
}
