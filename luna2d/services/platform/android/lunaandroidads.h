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
#include "lunaandroidjni.h"

namespace luna2d{

class LUNAAndroidAdsService : public LUNAAdsService
{
public:
	LUNAAndroidAdsService(const std::string& javaClassPath);
	
private:
	jclass javaClass = nullptr;
	jobject javaObject = nullptr;
	jmethodID javaGetBannerHeight = nullptr;
	jmethodID javaIsBannerShown = nullptr;
	jmethodID javaShowBanner = nullptr;
	jmethodID javaHideBanner = nullptr;
	jmethodID javaIsInterstitialReady = nullptr;
	jmethodID javaCacheInterstitial = nullptr;
	jmethodID javaShowInterstitial = nullptr;
	jmethodID javaIsRewardedVideoReady = nullptr;
	jmethodID javaCacheRewardedVideo = nullptr;
	jmethodID javaShowRewardedVideo = nullptr;
	bool isLoaded = false;

public:
	// Check for java part of ads service was loaded successufully
	bool IsLoaded();

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


class LUNAAndroidAds : public LUNAAds
{
public:
	// Load service instance by name
	virtual std::shared_ptr<LUNAAdsService> LoadService(const std::string& name);
};

}
