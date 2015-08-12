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

#include "lunasquirrel.h"

namespace luna2d{

enum class LUNAAssetType
{
	TEXTURE,
	TEXTURE_REGION,
	FONT
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
	LUNAAssetType type;

public:
	LUNAAssetType GetType();

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
	virtual void Reload() {} // Reload asset data
#endif
};


// On squiirel side, assets in most cases using as weak_ptr
// So, SqStack specialization for std::shared_ptr<LUNAAsset> is not need
template<>
struct SqStack<std::shared_ptr<LUNAAsset>>
{
	static void Push(HSQUIRRELVM vm, const std::shared_ptr<LUNAAsset>& value) = delete;
	static std::shared_ptr<LUNAAsset> Get(HSQUIRRELVM vm, int index = -1) = delete;
};


template<>
struct SqStack<std::weak_ptr<LUNAAsset>>
{
	static void Push(HSQUIRRELVM vm, const std::weak_ptr<LUNAAsset>& value);
	static std::weak_ptr<LUNAAsset> Get(HSQUIRRELVM vm, int index = -1);
};

}
