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
#import "lunaiosadsprotocol.h"

using namespace luna2d;

LUNAIosAdsService::LUNAIosAdsService(id service) :
	service(service)
{
	[service setOnRewardedVideoSuccess: [this]()
	{
		LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoSuccess();
	}];
	
	[service setOnRewardedVideoFail: [this]()
	{
		LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoFail();
	}];
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
}


// Load service instance by name
std::shared_ptr<LUNAAdsService> LUNAIosAds::LoadService(const std::string& name)
{
	id service = LUNAIosServices::LoadService(name, @protocol(LUNAIosAdsProtocol));
	
	if(!service) return nullptr;
	
	return std::make_shared<LUNAIosAdsService>(service);
}
