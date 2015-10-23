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

#include "lunaassets.h"
#include "lunagraphics.h"
#include "lunasizes.h"
#include "lunatextureatlasloader.h"
#include "lunafontloader.h"
#include "lunajsonloader.h"
#include "lunaaudiowavloader.h"
#include "lunaaudiooggloader.h"

using namespace luna2d;

LUNAAssets::LUNAAssets() :
	tblAssets(LUNAEngine::SharedLua())
{
	LuaScript* lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	// Bind assets table to lua
	tblLuna.SetField("assets", tblAssets);
	tblAssets.MakeReadOnly();

	// Bind assets manager to lua
	LuaTable tblAssetsMgr(lua);
	tblAssetsMgr.SetField("loadAll", LuaFunction(lua, this, &LUNAAssets::LoadAll));
	tblAssetsMgr.SetField("loadFolder", LuaFunction(lua, this, &LUNAAssets::LoadFolder));
	tblAssetsMgr.SetField("load", LuaFunction(lua, this, &LUNAAssets::Load));
	tblAssetsMgr.SetField("unload", LuaFunction(lua, this, &LUNAAssets::Unload));
	tblAssetsMgr.SetField("unloadFolder", LuaFunction(lua, this, &LUNAAssets::UnloadFolder));
	tblAssetsMgr.SetField("unloadAll", LuaFunction(lua, this, &LUNAAssets::UnloadAll));
	tblLuna.SetField("assetsmgr", tblAssetsMgr);

	// Bind base asset type
	LuaClass<LUNAAsset> clsAsset(lua);
}

LUNAAssets::~LUNAAssets()
{
	UnloadAll();
}

// Get parent table for given asset path
// Returns nil if path not found
// if "autoMake" is true, automatically creates non-existent tables
LuaTable LUNAAssets::GetParentTableForPath(const std::string& path, bool autoMake)
{
	if(path.empty()) return tblAssets;

	LuaTable ret = tblAssets;
	size_t prevPos = 0;
	size_t lastPos = path.rfind('/');

	// Parse given path and move by the tables tree
	while(prevPos < lastPos && ret)
	{
		size_t pos = path.find('/', prevPos);
		std::string name = path.substr(prevPos, pos - prevPos);
		LuaTable nextTable = ret.GetTable(name);

		// Make table if it isn't exists
		if(autoMake && !nextTable)
		{
			nextTable = LuaTable(LUNAEngine::SharedLua());
			nextTable.MakeReadOnly();
			ret.SetField(name, nextTable, true);
		}

		ret = nextTable;
		prevPos = pos + 1;
	}

	return ret;
}

// Get asset/folder name for path
std::string LUNAAssets::GetNameForPath(const std::string& path)
{
	return LUNAEngine::SharedFiles()->GetBasename(path);
}

// Check for given file should be ignored when loading
bool LUNAAssets::IsIgnored(const std::string& path)
{
	LUNAFiles* files = LUNAEngine::SharedFiles(); 

	if(path == CONFIG_FILENAME) return true; // Ignore config file
	if(path == SCRIPTS_PATH) return true; // Ignore scripts folder
	if(path == LOCALIZATION_PATH) return true; // Ignore localization folder

	// Ignore description files
	std::string ext = files->GetExtension(path);
	if(ext == "atlas" || ext == "font") return true;

	// Ignore files with different from current resolution suffix
	std::string suffix = files->SplitSuffix(files->GetBasename(path)).second;
	if(!suffix.empty() && suffix != LUNAEngine::SharedSizes()->GetResolutionSuffix()) return true;

	return false;
}

// Get loader for given file
std::shared_ptr<LUNAAssetLoader> LUNAAssets::GetLoader(const std::string& path)
{
	std::string ext = LUNAEngine::SharedFiles()->GetExtension(path);

	if(ext == "png")
	{
		// Load texture atlas if atlas desctiption file is exists
		std::string atlasPath = path.substr(0, path.rfind(".") + 1) + "atlas";
		if(LUNAEngine::SharedFiles()->IsFile(atlasPath)) return std::make_shared<LUNATextureAtlasLoader>();

		// Load just texture
		else return std::make_shared<LUNATextureLoader>();
	}
	else if(ext == "ttf") return std::make_shared<LUNAFontLoader>();
	else if(ext == "json") return std::make_shared<LUNAJsonLoader>();
	else if(ext == "wav") return std::make_shared<LUNAAudioWavLoader>();
	else if(ext == "ogg") return std::make_shared<LUNAAudioOggLoader>();

	return nullptr;
}

void LUNAAssets::DoLoadFile(const std::string& path)
{
	if(IsIgnored(path)) return; // Skip ignored files

	LUNAFiles* files = LUNAEngine::SharedFiles();

	auto loader = GetLoader(path);
	if(!loader)
	{
		LUNA_LOGE("Unknown asset type \"%s\"", files->GetExtension(path).c_str());
		return;
	}

	LuaTable parentTable = GetParentTableForPath(path, true);
	std::string name = files->SplitSuffix(files->GetBasename(path)).first; // Remove resolution suffix

	// Don't load asset if it already exists
	if(parentTable.HasField(name)) return;

	if(!loader->Load(path))
	{
		LUNA_LOGE("Cannot load asset from file \"%s\"", path.c_str());
		return;
	}

	loader->PushToLua(name, parentTable);
}


void LUNAAssets::DoUnloadFolder(LuaTable table)
{
	if(table.GetMetatable()) return; // This table is not asset folder

	// Recursively remove all assets in folder
	for(auto entry : table)
	{
		if(entry.second.GetType() == LUA_TTABLE) DoUnloadFolder(entry.second.ToTable());
		else
		{
			auto asset = entry.second.To<std::shared_ptr<LUNAAsset>>();
			if(asset) asset->_KillLuaRef();
		}

		table.RemoveField(entry.first.ToString(), true);
	}

	// Remove custom data if exists
	LuaTable meta = table.GetMetatable();
	auto customData = meta.GetField<std::shared_ptr<LUNAAsset>>(ASSET_CUSTOM_DATA_NAME);
	if(customData) customData->_KillLuaRef();
	meta.RemoveField(ASSET_CUSTOM_DATA_NAME);
}

 // Load all assets
void LUNAAssets::LoadAll()
{
	LoadFolder("", true);
}

// Load all assets in given folder
void LUNAAssets::LoadFolder(const std::string& path, bool recursive)
{
	if(IsIgnored(path)) return;

	LUNAFiles* files = LUNAEngine::SharedFiles();

	if(!files->IsDirectory(path))
	{
		LUNA_LOGE("Cannot load folder \"%s\". Folder not found", path.c_str());
		return;
	}

	for(std::string filename : files->GetFileList(path))
	{
		std::string fullPath = path + filename;

		if(files->IsFile(fullPath)) DoLoadFile(fullPath);
		else if(recursive) LoadFolder(fullPath, recursive);
	}
}

// Load specifed asset file
void LUNAAssets::Load(const std::string& path)
{
	LUNAFiles* files = LUNAEngine::SharedFiles();

	auto name = files->SplitSuffix(path);
	if(!name.second.empty())
	{
		LUNA_LOGE("Filename must be specifed without resolution suffix");
		return;
	}

	// Try load file without current resolution suffix
	if(files->IsFile(path))
	{
		DoLoadFile(path);
		return;
	}

	std::string suffixPath = files->AppendSuffix(path, LUNAEngine::SharedSizes()->GetResolutionSuffix());
	if(!files->IsFile(suffixPath))
	{
		LUNA_LOGE("File \"%s\" not found", path.c_str());
		return;
	}

	// Try load file with suffix
	DoLoadFile(suffixPath);
}

// Unload specifed asset
void LUNAAssets::Unload(const std::string& path)
{
	LuaTable parentTable = GetParentTableForPath(path);
	std::string name = GetNameForPath(path);

	if(parentTable && parentTable.HasField(name))
	{
		auto asset = parentTable.GetField<std::shared_ptr<LUNAAsset>>(name);
		asset->_KillLuaRef();
		parentTable.RemoveField(name, true);
	}
}

// Unload all assets in given folder
void LUNAAssets::UnloadFolder(const std::string& path)
{
	if(path.empty())
	{
		DoUnloadFolder(tblAssets);
		return;
	}

	LuaTable parentTable = GetParentTableForPath(path);
	std::string name = GetNameForPath(path);

	if(parentTable && parentTable.HasField(name))
	{
		DoUnloadFolder(parentTable.GetTable(name));
		parentTable.RemoveField(name);
	}
}

// Unload all assets
void LUNAAssets::UnloadAll()
{
	UnloadFolder("");
}

// Get root table of asset tree
LuaTable LUNAAssets::GetRootTable()
{
	return tblAssets;
}
