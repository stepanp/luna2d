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

#include "lunatextureatlasloader.h"

using namespace luna2d;

bool LUNATextureAtlasLoader::Load(const std::string& filename)
{
	// Description file for atlas has same name as image, just with different extension
	std::string atlasPath = LUNAEngine::SharedFiles()->ReplaceExtension(filename, "atlas");

	// Load texture
	LUNATextureLoader textureLoader;
	if(!textureLoader.Load(filename)) return false;
	texture = textureLoader.GetTexture();

	// Load texture atlas
	atlas = std::make_shared<LUNATextureAtlas>(texture, atlasPath);
	if(!atlas->IsLoaded()) return false;

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Set reload path for texture
	texture->SetReloadPath(filename);
#endif

	return true;
}

void LUNATextureAtlasLoader::PushToLua(const std::string& name, LuaTable& parentTable)
{
	// Make atlas table
	LuaTable atlasTable(LUNAEngine::SharedLua());
	atlasTable.MakeReadOnly();
	parentTable.SetField(name, atlasTable, true);

	// Add regions to asset table
	for(auto entry : atlas->GetRegions()) atlasTable.SetField(entry.first, entry.second, true);

	// Save texture in custom data of atlas table
	LUNAEngine::SharedAssets()->SetCustomDataToTable(atlasTable, texture);
}
