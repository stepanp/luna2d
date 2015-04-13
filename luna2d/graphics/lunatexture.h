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

#include "lunagl.h"
#include "lunaglhelpers.h"
#include "lunaimage.h"
#include "lunaassets.h"

namespace luna2d{

class LUNATexture : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNATexture)

public:
	LUNATexture(const LUNAImage& image);
	virtual ~LUNATexture();

private:
	int width, height;
	LUNAColorType colorType;
	GLuint id;

private:
	void CreateGlTexture(const std::vector<unsigned char>& data);

public:
	int GetWidth();
	int GetHeight();
	GLuint GetId();
	bool IsValid(); // Check for texture is valid. Can be invalid after loss GL context
	void Bind();
	void Unbind();

// Reload texture when application lost OpenGL context
// SEE: "lunaassets.h"
#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
private:
	std::string reloadPath; // Path to reload texture data
	bool cached = false; // Check if texture is generated and was cached to APP_FOLDER

public:
	inline void SetReloadPath(const std::string& path)
	{
		reloadPath = path;
		LUNAEngine::SharedAssets()->SetAssetReloadable(this, true); // Add this texture to reloadable assets
	}

	inline void SetCached(bool cached)
	{
		this->cached = cached;
	}

	virtual void Reload()
	{
		if(reloadPath.empty()) return;

		// Generated textures reloads direcly from cached data
		if(cached)
		{
			auto data = LUNAEngine::SharedFiles()->ReadFile(reloadPath, LUNAFileLocation::APP_FOLDER);
			if(!data.empty())
			{
				CreateGlTexture(data);
				return;
			}
		}

		// Usual textures reload from assets
		else
		{
			std::string ext = LUNAEngine::SharedFiles()->GetExtension(reloadPath);
			std::unique_ptr<LUNAImageFormat> format;

			// Select image format to decode
			if(ext == "png") format = std::unique_ptr<LUNAPngFormat>(new LUNAPngFormat());

			// Reload texture from path
			if(format)
			{
				LUNAImage image(reloadPath, *format, LUNAFileLocation::ASSETS);
				if(!image.IsEmpty())
				{
					CreateGlTexture(image.GetData());
					return;
				}
			}
		}

		LUNA_LOGE("Cannot reload texture from path \"%s\"", reloadPath.c_str());
	}
#endif

};

}
