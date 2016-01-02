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

#include "lunaads.h"

using namespace luna2d;

static LuaTable GetAdsTable()
{
	return LUNAEngine::SharedLua()->GetGlobalTable().GetTable("luna").GetTable("ads");
}

LUNAAds::LUNAAds()
{
	LuaScript* lua =  LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	LuaTable tblAds(lua);
	tblLuna.SetField("ads", tblAds);

	SetSdk(nullptr);

	tblAds.SetField("isVideoSupported", LuaFunction(lua, this, &LUNAAds::IsVideoSupported));
	tblAds.SetField("isVideoReady", LuaFunction(lua, this, &LUNAAds::IsVideoReady));
	tblAds.SetField("showVideo", LuaFunction(lua, this, &LUNAAds::ShowVideo));

}

void LUNAAds::OnVideoSuccess()
{
	LUNAEngine::Shared()->OnResume();

	auto tblAds = GetAdsTable();
	if(tblAds.HasField("onVideoSuccess")) tblAds.GetFunction("onVideoSuccess").CallVoid();
}

void LUNAAds::OnVideoFail()
{
	LUNAEngine::Shared()->OnResume();

	auto tblAds = GetAdsTable();
	if(tblAds.HasField("onVideoFail")) tblAds.GetFunction("onVideoFail").CallVoid();
}

bool LUNAAds::IsSdkSet()
{
	return sdk != nullptr;
}

void LUNAAds::SetSdk(const std::shared_ptr<LUNAAdsSdk>& sdk)
{
	this->sdk = sdk;

	if(sdk)
	{
		sdk->SetVideoCallbacks(
			std::bind(&LUNAAds::OnVideoSuccess, this),
			std::bind(&LUNAAds::OnVideoFail, this)
					);

		GetAdsTable().SetField("sdkname", sdk->GetName());
	}
	else GetAdsTable().RemoveField("sdkname");
}

bool LUNAAds::IsVideoSupported()
{
	return sdk && sdk->IsVideoSupported();
}

bool LUNAAds::IsVideoReady()
{
	return sdk && sdk->IsVideoSupported() && sdk->IsVideoReady();
}

void LUNAAds::ShowVideo()
{
	if(!IsVideoReady()) return;

	LUNAEngine::Shared()->OnPause();
	sdk->ShowVideo();
}
