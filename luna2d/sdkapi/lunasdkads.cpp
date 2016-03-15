//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

#include "lunasdkads.h"
#include "lunalua.h"

using namespace luna2d;

static LuaTable GetAdsTable()
{
	auto lua = LUNAEngine::SharedLua();
	auto tblLuna = lua->GetGlobalTable().GetTable("luna");
	return tblLuna.GetTable("ads");
}

void LUNAAds::AddVideoSdk(const std::shared_ptr<LUNAAdsSdk>& video)
{
	if(!video->IsVideoSupported()) LUNA_RETURN_ERR("SDK module \"%s\" not support video", video->GetName().c_str());

	this->video = video;
}

bool LUNAAds::IsVideoReady()
{
	return video && video->IsVideoReady();
}

void LUNAAds::CacheVideo()
{
	if(video) video->CacheVideo();
}

void LUNAAds::RequestVideo()
{
	if(!IsVideoReady()) return;

	video->ShowVideo();
}

void LUNAAds::OnVideoCompleted(const std::string& sdkName)
{
	auto tblAds = GetAdsTable();
	auto fnCallback = tblAds.GetFunction("onVideoCompleted");

	if(fnCallback != nil) fnCallback.CallVoid();

	CacheVideo();
}

void LUNAAds::OnVideoCanceled(const std::string& sdkName)
{
	auto tblAds = GetAdsTable();
	auto fnCallback = tblAds.GetFunction("onVideoCanceled");

	if(fnCallback != nil) fnCallback.CallVoid();

	CacheVideo();
}

void LUNAAds::OnVideoError(const std::string& sdkName, const std::string& error)
{
	auto tblAds = GetAdsTable();
	auto fnCallback = tblAds.GetFunction("onVideoError");

	if(fnCallback != nil) fnCallback.CallVoid(error);
}
