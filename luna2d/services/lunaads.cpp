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

#include "lunaads.h"
#include "lunaconfig.h"
#include "lunalua.h"
#include "lunasizes.h"

using namespace luna2d;

static LuaTable GetAdsTable()
{
	return LUNAEngine::SharedLua()->GetGlobalTable().GetTable("luna").GetTable("ads");
}

// Load services from config
void LUNAAds::LoadServices()
{
	auto config = LUNAEngine::Shared()->GetConfig();
	std::string serviceName = config->GetCustomValues()["adsService"].string_value();
	
	if(serviceName.empty()) return;
	
	service = LoadService(serviceName);
}

// Check for ads is enabled
bool LUNAAds::IsEnabled()
{
	return enabled;
}

// Enable/disable ads
void LUNAAds::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}

// Get default banner height (in pixels)
int LUNAAds::GetPhysicalBannerHeight()
{
	if(!service) return 0;

	return service->GetBannerHeight();
}

// Get default banner height (in points)
float LUNAAds::GetBannerHeight()
{
	auto sizes = LUNAEngine::SharedSizes();
	float ratio = sizes->GetScreenHeight() / (float)sizes->GetPhysicalScreenHeight();

	return GetPhysicalBannerHeight() * ratio;
}

// Check for banner is shown
bool LUNAAds::IsBannerShown()
{
	if(!service) return false;

	return service->IsBannerShown();
}

// Show banner
void LUNAAds::ShowBanner(const std::string& location)
{
	if(!service || !enabled) return;

	service->ShowBanner(location);
}

void LUNAAds::HideBanner()
{
	if(!service) return;

	service->HideBanner();
}

// Check for interstitial is downloaded ready to showing
bool LUNAAds::IsInterstitialReady()
{
	if(!service || !enabled) return false;

	return service->IsInterstitialReady();
}

// Cache interstitial
void LUNAAds::CacheInterstitial(const std::string& location)
{
	if(!service || !enabled) return;

	return service->CacheInterstitial(location);
}

// Show interstitial
void LUNAAds::ShowInterstitial(const std::string& location)
{
	if(!service || !enabled) return;

	service->ShowInterstitial(location);
}

// Called when interstitial has been closed
void LUNAAds::OnInterstitialClosed()
{
	auto tblAds = GetAdsTable();
	auto fnClosed = tblAds.GetFunction("onInterstitialClosed");

	if(fnClosed) fnClosed.CallVoid();
}

// Check for rewarded video is downloaded ready to showing
bool LUNAAds::IsRewardedVideoReady()
{
	if(!service || !enabled) return false;

	return service->IsRewardedVideoReady();
}

// Cache rewarded video
void LUNAAds::CacheRewardedVideo(const std::string& location)
{
	if(!service || !enabled) return;

	return service->CacheRewardedVideo(location);
}

// Show rewarded video
void LUNAAds::ShowRewardedVideo(const std::string& location)
{
	if(!service || !enabled || !service->IsRewardedVideoReady()) return;

	service->ShowRewardedVideo(location);
}

// Called when video has been succesfully viewed
void LUNAAds::OnRewardedVideoSuccess()
{
	auto tblAds = GetAdsTable();
	auto fnSuccess = tblAds.GetFunction("onRewardedVideoSuccess");
	
	if(fnSuccess) fnSuccess.CallVoid();
}

// Called when video has been dismissed or in case of error
void LUNAAds::OnRewardedVideoFail()
{
	auto tblAds = GetAdsTable();
	auto fnFail = tblAds.GetFunction("onRewardedVideoFail");
	
	if(fnFail) fnFail.CallVoid();
}
