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
#include <json11.hpp>

using namespace luna2d;
using namespace json11;

bool LUNAFontLoader::Load(const std::string& filename, const std::string& normalizedPath,
	std::unordered_map<std::string, std::shared_ptr<LUNAAsset>>& loadedAssets)
{
	LUNAFiles* files = LUNAEngine::SharedFiles();

	// Load description file for font
	std::string desc = files->ReplaceExtension(filename, "font");
	if(!files->IsFile(desc)) return false;

	std::string descData = files->ReadFileToString(desc);
	std::string err;
	Json jsonDesc = Json::parse(descData, err);
	if(jsonDesc == nullptr)
	{
		LUNA_LOGE(err.c_str());
		return false;
	}

	// Load font file
	LUNAFontGenerator generator;
	if(!generator.Load(filename)) return false;

	if(jsonDesc.object_items().size() == 0) return false;

	// Generate bitmap fonts for each specifed size in description file
	auto folderPath = normalizedPath + "/";
	for(auto entry : jsonDesc.object_items())
	{
		auto fontPath = folderPath + entry.first;
		loadedAssets[fontPath] = generator.GenerateFont(entry.second.int_value());
	}

	// Make folder item in loaded assets
	// for can be checked that this asset already loaded
	loadedAssets[folderPath] = nullptr;

	return true;
}
