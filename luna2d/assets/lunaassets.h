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

#pragma once

#include "lunaengine.h"
#include "lunalua.h"
#include "lunafiles.h"

namespace luna2d{

const std::string ASSET_CUSTOM_DATA_NAME = "_customData"; // Name of field in asset table with custom data

//----------------------
// Base class for assets
//----------------------
class LUNAAsset
{
	LUNA_USERDATA(LUNAAsset)

public:
	virtual ~LUNAAsset() {}

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
public:
	virtual void Reload() {} // Reload asset data
	virtual void Cache() {} // Cache asset data
#endif
};

//-----------------------------
// Base class for asset loaders
//-----------------------------
class LUNAAssetLoader
{
public:
	virtual ~LUNAAssetLoader() {}

public:
	virtual bool Load(const std::string& filename) = 0;
	virtual void PushToLua(const std::string& name, LuaTable& parentTable) = 0;
};

//---------------
// Assets manager
//---------------
class LUNAAssets
{
public:
	LUNAAssets();
	~LUNAAssets();

private:
	LuaTable tblAssets; // Root table of asset tree in lua

private:
	// Get parent table for given asset path
	// Returns nil if path not found
	// if "autoMake" is true, automatically creates non-existent tables
	LuaTable GetParentTableForPath(const std::string& path, bool autoMake = false);
	std::string GetNameForPath(const std::string& path); // Get asset/folder name for path
	bool IsIgnored(const std::string& path); // Check for given file should be ignored when loading
	std::shared_ptr<LUNAAssetLoader> GetLoader(const std::string& path); // Get loader for given file

	void DoLoadFile(const std::string& path);
	void DoUnloadFolder(LuaTable table);

public:
	void LoadAll(); // Load all assets
	void LoadFolder(const std::string& path, bool recursive = false); // Load all assets in given folder
	void Load(const std::string& path); // Load specifed asset file
	void Unload(const std::string& path); // Unload specifed asset
	void UnloadFolder(const std::string& path); // Unload all assets in given folder
	void UnloadAll(); // Unload all assets

	LuaTable GetRootTable(); // Get root table of asset tree

	// Get asset by path like "folder/folder/asset"
	// If asset not fount or isn't instance of given "AssetType" class, return nullptr
	template<typename AssetType>
	std::weak_ptr<AssetType> GetAssetByPath(const std::string& path)
	{
		LuaTable parent = GetParentTableForPath(path);
		if(!parent) return std::weak_ptr<AssetType>();

		return parent.GetField<std::weak_ptr<AssetType>>(GetNameForPath(path));
	}

	// Helper for set custom data to asset table
	template<typename DataType>
	void SetCustomDataToTable(LuaTable& table, DataType& data)
	{
		LuaTable meta = table.GetMetatable();
		meta.SetField(ASSET_CUSTOM_DATA_NAME, data);
	}

// On Android application may lost OpenGL context on pause
// In that case we need reload all assets associated with this context(textures, shaders, etc.)
#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
private:
	std::unordered_set<LUNAAsset*> reloadableAssets; // List of all assets needing reload
	int lastCachedId = 0; // Counter for generate unique cached texture names
	std::stack<int> freeCacheIds;

public:
	inline int NewCacheId()
	{
		if(freeCacheIds.empty())
		{
			lastCachedId++;
			return lastCachedId;
		}

		int cacheId = freeCacheIds.top();
		freeCacheIds.pop();
		return cacheId;
	}

	inline void ReleaseCacheId(int cacheId)
	{
		freeCacheIds.push(cacheId);
	}

	// Cache generated texture to APP_DATA folder
	// Return path to resulting file
	inline int CacheTexture(const std::vector<unsigned char>& textureData, int cacheId = 0)
	{
		cacheId = cacheId != 0 ? cacheId : NewCacheId();
		std::string path = ".luna2d_gentexture_" + std::to_string(cacheId);

		if(!LUNAEngine::SharedFiles()->WriteCompressedFile(path, textureData, LUNAFileLocation::APP_FOLDER)) return 0;

		return cacheId;
	}

	// Add/remove asset to list of reloadable asets
	inline void SetAssetReloadable(LUNAAsset* asset, bool reloadable)
	{
		if(reloadable) reloadableAssets.insert(asset);
		else reloadableAssets.erase(asset);
	}

	inline void ReloadAssets()
	{
		for(LUNAAsset* asset : reloadableAssets) asset->Reload();
	}

	inline void CacheAssets()
	{
		for(LUNAAsset* asset : reloadableAssets) asset->Cache();
	}
#endif
};

}
