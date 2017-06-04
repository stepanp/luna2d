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

#pragma once

#include "lunaads.h"

namespace luna2d{
	
class LUNAIosAdsService : public LUNAAdsService
{
public:
	LUNAIosAdsService(id service);
	
private:
	id service;
	
public:
	// Get default banner height (in pixels)
	virtual int GetBannerHeight();
	
	// Check for banner is shown
	virtual bool IsBannerShown();
	
	// Show banner
	virtual void ShowBanner(const std::string& location);
	
	// Hide banner
	virtual void HideBanner();
	
	// Check for interstitial is downloaded ready to showing
	virtual bool IsInterstitialReady();
	
	// Cache interstitial
	virtual void CacheInterstitial(const std::string& location);
	
	// Show interstitial
	virtual void ShowInterstitial(const std::string& location);
	
	// Check for video is downloaded ready to showing
	virtual bool IsRewardedVideoReady();
	
	// Cache rewarded video
	virtual void CacheRewardedVideo(const std::string& location);
	
	// Show rewarded video
	virtual void ShowRewardedVideo(const std::string& location);
};

	
class LUNAIosAds : public LUNAAds
{
public:
	// Load service instance by name
	virtual std::shared_ptr<LUNAAdsService> LoadService(const std::string& name);
};

}
