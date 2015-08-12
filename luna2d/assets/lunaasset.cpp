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

#include "lunaasset.h"
#include "lunatextureregion.h"
#include "lunafont.h"

using namespace luna2d;

LUNAAsset::LUNAAsset(LUNAAssetType type) : 
	type(type)
{
}

LUNAAssetType LUNAAsset::GetType()
{
	return type;
}


void SqStack<std::weak_ptr<LUNAAsset>>::Push(HSQUIRRELVM vm, const std::weak_ptr<LUNAAsset>& value)
{
	if(value.expired())
	{
		sq_pushnull(vm);
		return;
	}

	// In "LUNAAssets" all assets stored as "std::shared_ptr<LUNAAsset>"
	// But for pushing to squirrel we need know concrete type at compile-time
	auto valueShared = value.lock();
	switch(valueShared->GetType())
	{
	case LUNAAssetType::TEXTURE:
		SqStack<std::weak_ptr<LUNATexture>>::Push(vm, std::static_pointer_cast<LUNATexture>(valueShared));
		break;
	case LUNAAssetType::TEXTURE_REGION:
		SqStack<std::weak_ptr<LUNATextureRegion>>::Push(vm, std::static_pointer_cast<LUNATextureRegion>(valueShared));
		break;
	case LUNAAssetType::FONT:
		SqStack<std::weak_ptr<LUNAFont>>::Push(vm, std::static_pointer_cast<LUNAFont>(valueShared));
		break;
	default:
		sq_pushnull(vm);
	}
}

std::weak_ptr<LUNAAsset> SqStack<std::weak_ptr<LUNAAsset>>::Get(HSQUIRRELVM vm, int index)
{
	if(sq_gettype(vm, index) != OT_INSTANCE) return std::weak_ptr<LUNAAsset>();

	size_t typeTag = SqClassInfo<LUNAAsset>::GetTypeTag();
	if(typeTag == 0) return std::weak_ptr<LUNAAsset>();

	SQUserPointer ptr = nullptr;
	if(SQ_FAILED(sq_getinstanceup(vm, index, &ptr, reinterpret_cast<SQUserPointer>(typeTag)))) return std::weak_ptr<LUNAAsset>();

	if(!ptr) return std::weak_ptr<LUNAAsset>();

	return static_cast<SqPtr<LUNAAsset>*>(ptr)->ToWeak();
}
