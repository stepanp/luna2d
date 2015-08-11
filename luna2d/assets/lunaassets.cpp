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
#include "lunatextureloader.h"
#include "lunatextureatlasloader.h"
#include "lunafontloader.h"

using namespace luna2d;

LUNAAssets::LUNAAssets() :
	tblAssets(LUNAEngine::SharedSquirrel())
{
	SqVm* sq = LUNAEngine::SharedSquirrel();
	SqTable tblLuna = sq->GetRootTable().GetTable("luna");

	// Bind assets table to squirrel
	SqTable delegate(sq);
	std::function<std::weak_ptr<LUNAAsset>(const std::string&)> getter = [&](const std::string& path)
	{
		auto it = loadedAssets.find(path);
		if(it == loadedAssets.end()) return std::weak_ptr<LUNAAsset>();
		return std::weak_ptr<LUNAAsset>(it->second);
	};
	delegate.NewSlot("_get", SqFunction(sq, getter));
	tblAssets.SetDelegate(delegate);

	tblLuna.NewSlot("assets", tblAssets);

	// Bind assets manager to squirrel
	SqTable tblAssetsMgr(sq);
	tblAssetsMgr.NewSlot("loadAll", SqFunction(sq, this, &LUNAAssets::LoadAll));
	tblAssetsMgr.NewSlot("loadFolder", SqFunction(sq, this, &LUNAAssets::LoadFolder));
	tblAssetsMgr.NewSlot("load", SqFunction(sq, this, &LUNAAssets::Load));
	tblAssetsMgr.NewSlot("unload", SqFunction(sq, this, &LUNAAssets::Unload));
	tblAssetsMgr.NewSlot("unloadFolder", SqFunction(sq, this, &LUNAAssets::UnloadFolder));
	tblAssetsMgr.NewSlot("unloadAll", SqFunction(sq, this, &LUNAAssets::UnloadAll));
	tblLuna.NewSlot("assetsmgr", tblAssetsMgr);

	// Bind base asset type
	SqClass<LUNAAsset> clsAsset(sq);
}

LUNAAssets::~LUNAAssets()
{
	UnloadAll();
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

	return nullptr;
}

void LUNAAssets::DoLoadFile(const std::string& path)
{
	if(IsIgnored(path)) return; // Skip ignored files

	LUNAFiles* files = LUNAEngine::SharedFiles();

	// Crop extension and "@" suffix
	std::string normalizedPath = files->SplitSuffix(path).first;

	// Don't load asset if it already exists
	if(loadedAssets.count("normalizedPath") != 0) return;

	auto loader = GetLoader(path);
	if(!loader)
	{
		LUNA_LOGE("Unknown asset type \"%s\"", files->GetExtension(path).c_str());
		return;
	}

	if(!loader->Load(path, normalizedPath, loadedAssets))
	{
		LUNA_LOGE("Cannot load asset from file \"%s\"", path.c_str());
		return;
	}
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
	if(path.empty() || path[path.size() - 1] == '/') return; // Do not unload folders

	auto it = loadedAssets.find(path);
	if(it != loadedAssets.end()) loadedAssets.erase(it);
}

// Unload all assets in given folder
void LUNAAssets::UnloadFolder(const std::string& path)
{
	if(path.empty() || path[path.size() - 1] != '/') return; // Unload only folders

	// Remove all assets having given path in id
	auto it = loadedAssets.begin();
	for(; it != loadedAssets.end();)
	{
		if(std::equal(path.begin(), path.end(), it->first.begin())) it = loadedAssets.erase(it);
		else ++it;
	}

	// Remove folder item
	auto folderIt = loadedAssets.find(path);
	if(folderIt != loadedAssets.end()) loadedAssets.erase(folderIt);
}

// Unload all assets
void LUNAAssets::UnloadAll()
{
	loadedAssets.clear();
}
