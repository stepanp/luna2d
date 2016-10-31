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

#include "lunaiosads.h"
#include "lunaiosservices.h"
#include "lunaplatformutils.h"
#include "lunalog.h"
#import "lunaiosadsprotocol.h"

using namespace luna2d;

LUNAIosAdsService::LUNAIosAdsService(id service) :
	service(service)
{
	[service setOnRewardedVideoSuccess: ^(void)
	{
		LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(false);
		LUNAEngine::Shared()->EnablePauseHandling(true);
		LUNAEngine::Shared()->OnResume();
		LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoSuccess();
	}];
	
	[service setOnRewardedVideoFail: ^(void)
	{
		LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(false);
		LUNAEngine::Shared()->EnablePauseHandling(true);
		LUNAEngine::Shared()->OnResume();
		LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoFail();
	}];
	
	[service setOnRewardedVideoError: ^(void)
	{
		LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(false);
		LUNAEngine::SharedPlatformUtils()->MessageDialog("Error", "Cannot load rewarded video",
			[]()
			{
				LUNAEngine::Shared()->EnablePauseHandling(true);
				LUNAEngine::Shared()->OnResume();
				LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoFail();
			});
	}];
}

// Check for interstitial is downloaded ready to showing
bool LUNAIosAdsService::IsInterstitalReady()
{
	return [service isInterstitialReady];
}

// Check for video is downloaded ready to showing
bool LUNAIosAdsService::IsRewardedVideoReady()
{
	return [service isRewardedVideoReady];
}

// Show interstitial
void LUNAIosAdsService::ShowInterstital()
{
	[service showInterstitial];
}

// Show rewarded video
void LUNAIosAdsService::ShowRewardedVideo()
{
	[service showRewardedVideo];
	LUNAEngine::SharedPlatformUtils()->ShowLoadingIndicator(true);
	LUNAEngine::Shared()->OnPause();
	
	// Protect game from resuming by operating system
	LUNAEngine::Shared()->EnablePauseHandling(false);
}


// Load service instance by name
std::shared_ptr<LUNAAdsService> LUNAIosAds::LoadService(const std::string& name)
{
	id service = LUNAIosServices::LoadService(name, @protocol(LUNAIosAdsProtocol));
	
	if(!service) return nullptr;
	
	return std::make_shared<LUNAIosAdsService>(service);
}
