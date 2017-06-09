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

package com.stepanp.luna2d.services.api;

public abstract class LunaAdsService
{
	// Get default banner height (in pixels)
	public abstract int getBannerHeight();

	// Check for banner is shown
	public abstract boolean isBannerShown();

	// Show banner
	public abstract void showBanner(String location);

	// Hide banner
	public abstract void hideBanner();	

	// Check for interstitial is downloaded ready to showing
	public abstract boolean isInterstitialReady();

	// Cache interstitial
	public abstract void cacheInterstitial(String location);

	// Show interstitial
	public abstract void showInterstitial(String location);

	// Check for rewarded video is downloaded ready to showing
	public abstract boolean isRewardedVideoReady();

	// Cache rewarded video
	public abstract void cacheRewardedVideo(String location);

	// Show rewarded video
	public abstract void showRewardedVideo(String location);

	// Callback for interstitial has been closed
	protected native void onInterstitialClosed();

	// Callback for video has been succesfully viewed
	protected native void onRewardedVideoSuccess();

	// Callback for video has been dismissed
	protected native void onRewardedVideoFail();

	// Callback for video cause error
	protected native void onRewardedVideoError();
}
