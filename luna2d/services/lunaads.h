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

#include "lunaengine.h"

namespace luna2d{

class LUNAAdsService
{
public:
	virtual ~LUNAAdsService() {}

public:
	// Get default banner height (in pixels)
	virtual int GetBannerHeight() = 0;

	// Check for banner is shown
	virtual bool IsBannerShown() = 0;

	// Show banner
	virtual void ShowBanner(const std::string& location) = 0;

	// Hide banner
	virtual void HideBanner() = 0;

	// Check for interstitial is downloaded ready to showing
	virtual bool IsInterstitialReady() = 0;

	// Cache interstitial
	virtual void CacheInterstitial(const std::string& location) = 0;

	// Show interstitial
	virtual void ShowInterstitial(const std::string& location) = 0;

	// Check for video is downloaded ready to showing
	virtual bool IsRewardedVideoReady() = 0;

	// Cache rewarded video
	virtual void CacheRewardedVideo(const std::string& location) = 0;

	// Show rewarded video
	virtual void ShowRewardedVideo(const std::string& location) = 0;
};


class LUNAAds
{
public:
	virtual ~LUNAAds() {}

private:
	std::shared_ptr<LUNAAdsService> service;

public:
	// Load service instance by name
	virtual std::shared_ptr<LUNAAdsService> LoadService(const std::string& name) = 0;

	// Load services from config
	void LoadServices();

	// Get default banner height (in pixels)
	int GetPhysicalBannerHeight();

	// Get default banner height (in points)
	float GetBannerHeight();

	// Check for banner is shown
	bool IsBannerShown();

	// Show banner
	void ShowBanner(const std::string& location);

	// Hide banner
	void HideBanner();

	// Check for interstitial is downloaded ready to showing
	bool IsInterstitialReady();

	// Cache interstitial
	void CacheInterstitial(const std::string& location);

	// Show interstitial
	void ShowInterstitial(const std::string& location);

	// Called when interstitial has been closed
	void OnInterstitialClosed();

	// Check for rewarded video is downloaded ready to showing
	bool IsRewardedVideoReady();

	// Cache rewarded video
	void CacheRewardedVideo(const std::string& location);

	// Show rewarded video
	void ShowRewardedVideo(const std::string& location);

	// Called when video has been succesfully viewed
	void OnRewardedVideoSuccess();

	// Called when video has been dismissed or in case of error
	void OnRewardedVideoFail();
};

}
