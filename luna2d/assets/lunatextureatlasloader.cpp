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

#include "lunatextureatlasloader.h"
#include "lunatextureloader.h"

using namespace luna2d;

bool LUNATextureAtlasLoader::Load(const std::string& filename, const std::string& normalizedPath,
	std::unordered_map<std::string, std::shared_ptr<LUNAAsset>>& loadedAssets)
{
	auto texture = LUNATextureLoader::LoadTexture(filename);
	if(!texture) return false;

	// Description file for atlas has same name as image, just with different extension
	std::string atlasPath = LUNAEngine::SharedFiles()->ReplaceExtension(filename, "atlas");

	// Load texture atlas
	auto atlas = std::make_shared<LUNATextureAtlas>(texture, atlasPath);
	if(!atlas->IsLoaded()) return false;

	auto folderPath = normalizedPath + "/";
	loadedAssets[folderPath] = texture;

	for(auto entry : atlas->GetRegions())
	{
		auto regionPath = folderPath + entry.first;
		loadedAssets[regionPath] = entry.second;
	}

	return true;
}
