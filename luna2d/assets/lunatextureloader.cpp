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

#include "lunatextureloader.h"

using namespace luna2d;

std::shared_ptr<LUNATexture> LUNATextureLoader::GetTexture()
{
	return texture;
}

bool LUNATextureLoader::Load(const std::string& filename)
{
	std::string ext = LUNAEngine::SharedFiles()->GetExtension(filename);
	std::unique_ptr<LUNAImageFormat> format;

	// Select image format to decode
	if(ext == "png") format = std::unique_ptr<LUNAPngFormat>(new LUNAPngFormat());
	if(!format) return false;

	// Load image data
	LUNAImage image(filename, *format, LUNAFileLocation::ASSETS);
	if(image.IsEmpty()) return false;

	// Make texture from image
	texture = std::make_shared<LUNATexture>(image);

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	// Set reload path for texture
	texture->SetReloadPath(filename);
#endif

	return true;
}

void LUNATextureLoader::PushToLua(const std::string& name, LuaTable& parentTable)
{
	parentTable.SetField(name, texture, true);
}
