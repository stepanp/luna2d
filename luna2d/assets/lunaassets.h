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

#pragma once

#include "lunaengine.h"
#include "lunafiles.h"
#include "lunasquirrel.h"

#include "lunalua.h"

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
#include "lunaimage.h"
#endif

namespace luna2d{

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
	virtual bool Load(const std::string& filename, const std::string& normalizedPath,
		std::unordered_map<std::string, std::shared_ptr<LUNAAsset>>& loadedAssets) = 0;
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
	std::unordered_map<std::string, std::shared_ptr<LUNAAsset>> loadedAssets;
	SqTable tblAssets;

private:
	bool IsIgnored(const std::string& path); // Check for given file should be ignored when loading
	std::shared_ptr<LUNAAssetLoader> GetLoader(const std::string& path); // Get loader for given file
	void DoLoadFile(const std::string& path);

public:
	void LoadAll(); // Load all assets
	void LoadFolder(const std::string& path, bool recursive = false); // Load all assets in given folder
	void Load(const std::string& path); // Load specifed asset file
	void Unload(const std::string& path); // Unload specifed asset
	void UnloadFolder(const std::string& path); // Unload all assets in given folder
	void UnloadAll(); // Unload all assets


// On Android application may lost OpenGL context on pause
// In that case we need reload all assets associated with this context(textures, shaders, etc.)
#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
private:
	std::unordered_set<LUNAAsset*> reloadableAssets; // List of all assets needing reload
	int lastCachedId = 0; // Counter for generate unique cached texture names

public:
	// Cache generated texture to APP_DATA folder
	// Return path to resulting file
	inline std::string CacheTexture(const LUNAImage& image)
	{
		std::string path = ".luna2d_gentexture_" + std::to_string(lastCachedId);
		if(!LUNAEngine::SharedFiles()->WriteCompressedFile(path, image.GetData(), LUNAFileLocation::APP_FOLDER)) return "";
		lastCachedId++;
		return path;
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
#endif
};

}
