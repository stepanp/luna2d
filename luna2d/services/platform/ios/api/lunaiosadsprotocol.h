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

@protocol LUNAIosAdsProtocol

// Set callback calling when interstitial has been closed
-(void) setOnInterstitialClosed: (void (^)()) callback;

// Set callback calling when video has been succesfully viewed
-(void) setOnRewardedVideoSuccess: (void (^)()) callback;

// Set callback calling when video has been dismissed
-(void) setOnRewardedVideoFail: (void (^)()) callback;

// Set callback calling when video cause error
-(void) setOnRewardedVideoError: (void (^)()) callback;

// Get default banner height (in pixels)
-(int) getBannerHeight;

// Check for banner is shown
-(BOOL) isBannerShown;

// Show banner
-(void) showBanner: (NSString*) location;

// Hide banner
-(void) hideBanner;

// Show interstitial
-(void) showInterstitial: (NSString*) location;

// Check for interstitial is downloaded ready to showing
-(BOOL) isInterstitialReady;

// Cache interstitial
-(void) cacheInterstitial: (NSString*) location;

// Check for rewarded video is downloaded ready to showing
-(BOOL) isRewardedVideoReady;

// Cache rewarded video
-(void) cacheRewardedVideo: (NSString*) location;

// Show rewarded video
-(void) showRewardedVideo: (NSString*) location;
	
@end
