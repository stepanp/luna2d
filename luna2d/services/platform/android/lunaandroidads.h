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
	jmethodID javaIsInterstitalReady = nullptr;
	jmethodID javaIsRewardedVideoReady = nullptr;
	jmethodID javaShowInterstital = nullptr;
	jmethodID javaShowRewardedVideo = nullptr;
	bool isLoaded = false;

public:
	// Check for java part of ads service was loaded successufully
	bool IsLoaded();

	// Check for interstitial is downloaded ready to showing
	virtual bool IsInterstitalReady();
	
	// Check for video is downloaded ready to showing
	virtual bool IsRewardedVideoReady();
	
	// Show interstitial
	virtual void ShowInterstital();
		
	// Show rewarded video
	virtual void ShowRewardedVideo();
};


class LUNAAndroidAds : public LUNAAds
{
public:
	// Load service instance by name
	virtual std::shared_ptr<LUNAAdsService> LoadService(const std::string& name);
};

}
