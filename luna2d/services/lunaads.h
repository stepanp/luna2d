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

	// Check for banner is downloaded ready to showing
	virtual bool IsBannerReady() = 0;

	// Check for interstitial is downloaded ready to showing
	virtual bool IsInterstitalReady() = 0;

	// Check for video is downloaded ready to showing
	virtual bool IsRewardedVideoReady() = 0;

	// Show banner
	virtual void ShowBanner(const std::string& location) = 0;

	// Show interstitial
	virtual void ShowInterstital(const std::string& location) = 0;

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

	// Check for banner is downloaded ready to showing
	bool IsBannerReady();

	// Check for interstitial is downloaded ready to showing
	bool IsInterstitalReady();

	// Check for rewarded video is downloaded ready to showing
	bool IsRewardedVideoReady();

	// Show banner
	void ShowBanner(const std::string& location);

	// Show interstitial
	void ShowInterstital(const std::string& location);

	// Show rewarded video
	void ShowRewardedVideo(const std::string& location);

	// Called when video has been succesfully viewed
	void OnRewardedVideoSuccess();

	// Called when video has been dismissed or in case of error
	void OnRewardedVideoFail();
};

}
