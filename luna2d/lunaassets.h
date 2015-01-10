//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#include "lunalua.h"
#include "lunamakeweak.h"
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

namespace luna2d{

class LUNATexture;

//------------
// Asset types
//------------
enum class LUNAAssetType
{
	TEXTURE,
	TEXTURE_REGION,
};

//----------------------
// Base class for assets
//----------------------
class LUNAAsset
{
public:
	LUNAAsset(LUNAAssetType type);
	virtual ~LUNAAsset() {}

protected:
	std::string reloadPath; // Path to reload asset data if OpenGL context was lost
	LUNAAssetType type;

public:
	LUNAAssetType GetType();
	std::string GetReloadPath();
	void SetReloadPath(const std::string& path);
	virtual void Reload() {} // Reload asset data
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
	LuaTable tblAssets; // Table representes loaded assets to lua
	std::unordered_map<int, std::shared_ptr<LUNAAsset>> loadedAssets; // List of all loaded assets
	int lastId;

private:
	// Load asset data
	// "filename" - Full path to loaded file
	// "cleanName" - Name of file without path and resolution suffix
	// "table" - Table into tblAssets where to place loaded asset
	void LoadAssetData(const std::string& filename, const std::string& cleanName, LuaTable& table);
	int GetNewId(); // Generate id for new asset
	void UpdateAssetsTable(LuaTable& table); // Remove unloaded assets and empty tables from given assets table
	void UnloadTable(LuaTable& table); // Recursively unload all assets in given table

public:
	void LoadAll(); // Load all assets
	void LoadFolder(const std::string& folder, bool recursive = false); // Load all assets in given folder
	void LoadFile(const std::string& filename); // Load specifed asset file
	void Unload(const LuaDynamicType& asset); // Unload given asset or folder
	void UnloadAll(); // Unload all assets
	void ReloadAssets(); // Reload some assets if OpenGL context was lost

	// Check type for assset at given id
	bool IsAssetA(int assetId, LUNAAssetType type);

	// Get asset by id with type-checking
	template<typename T>
	std::weak_ptr<T> GetAsset(int assetId, LUNAAssetType type)
	{
		if(!IsAssetA(assetId, type)) return std::weak_ptr<T>();
		return make_weak(std::static_pointer_cast<T>(loadedAssets[assetId]));
	}
};

}
