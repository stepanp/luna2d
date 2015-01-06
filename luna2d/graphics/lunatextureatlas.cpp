//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaengine.h"
#include "lunafiles.h"
#include "lunatextureatlas.h"
#include "lunalog.h"
#include <json11.hpp>

using namespace luna2d;
using namespace json11;

LUNATextureAtlas::LUNATextureAtlas(const std::shared_ptr<LUNATexture>& texture, const std::string& atlasFile,
	LUNAFileLocation location)
{
	Load(texture, atlasFile, location);
}

void LUNATextureAtlas::Load(const std::shared_ptr<LUNATexture>& texture, const std::string& atlasFile, LUNAFileLocation location)
{
	std::string atlasData = LUNAEngine::SharedFiles()->ReadFileToString(atlasFile, location);
	if(atlasData.empty()) return;

	std::string err;
	Json jsonAtlas = Json::parse(atlasData, err);
	if(jsonAtlas == nullptr)
	{
		LUNA_LOGE(err.c_str());
		return;
	}

	auto weakTexture = make_weak(texture);
	for(auto entry : jsonAtlas.object_items())
	{
		const std::string& name = entry.first;
		const Json& jsonRegion = entry.second;

		int x = jsonRegion["x"].int_value();
		int y = jsonRegion["y"].int_value();
		int width = jsonRegion["width"].int_value();
		int height = jsonRegion["height"].int_value();

		regions[name] = std::make_shared<LUNATextureRegion>(weakTexture, x, y, width, height);
	}
}

bool LUNATextureAtlas::IsLoaded() const
{
	return !regions.empty();
}

auto LUNATextureAtlas::GetRegions() -> decltype(LUNATextureAtlas::regions)
{
	return regions;
}
