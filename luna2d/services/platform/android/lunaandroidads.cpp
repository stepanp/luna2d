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

#include "lunaandroidads.h"
#include "lunalog.h"
#include "lunaplatformutils.h"
#include "lunaandroidservices.h"

using namespace luna2d;

LUNAAndroidAdsService::LUNAAndroidAdsService(const std::string& javaClassPath)
{
	// Convert classpath like "com.package.Class" 
	// to internal JNI format like "com/package/Class"
	auto jniClasspath = javaClassPath;
	std::replace(jniClasspath.begin(), jniClasspath.end(), '.', '/');

	jni::Env env;

	// Get ref to java wrapper class
	jclass localClassRef = env->FindClass(jniClasspath.c_str());
	if(env->ExceptionCheck()) 
	{
		env->ExceptionClear();
		LUNA_LOGE("Cannot load ads serivce. Java class \"%s\" not found", javaClassPath.c_str());
		return;
	}

	javaClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClassRef));
	env->DeleteLocalRef(localClassRef);

	// Make new object of java wrapper class
	jmethodID constructor = env->GetMethodID(javaClass, "<init>", "()V");
	jobject localObjRef = env->NewObject(javaClass, constructor);
	javaObject = reinterpret_cast<jobject>(env->NewGlobalRef(localObjRef));
	env->DeleteLocalRef(localObjRef);

	// Get java wrapper method ids
	javaGetBannerHeight = env->GetMethodID(javaClass, "getBannerHeight", "()I");
	javaIsBannerShown = env->GetMethodID(javaClass, "isBannerShown", "()Z");
	javaShowBanner = env->GetMethodID(javaClass, "showBanner", "(Ljava/lang/String;)V");
	javaHideBanner = env->GetMethodID(javaClass, "hideBanner", "()V");
	javaIsInterstitialReady = env->GetMethodID(javaClass, "isInterstitialReady", "()Z");
	javaCacheInterstitial = env->GetMethodID(javaClass, "cacheInterstitial", "(Ljava/lang/String;)V");
	javaShowInterstitial = env->GetMethodID(javaClass, "showInterstitial", "(Ljava/lang/String;)V");
	javaIsRewardedVideoReady = env->GetMethodID(javaClass, "isRewardedVideoReady", "()Z");
	javaCacheRewardedVideo = env->GetMethodID(javaClass, "cacheRewardedVideo", "(Ljava/lang/String;)V");
	javaShowRewardedVideo = env->GetMethodID(javaClass, "showRewardedVideo", "(Ljava/lang/String;)V");

	isLoaded = true;
}

// Check for java part of ads service was loaded successufully
bool LUNAAndroidAdsService::IsLoaded()
{
	return isLoaded;
}

// Get default banner height (in pixels)
int LUNAAndroidAdsService::GetBannerHeight()
{
	return jni::Env()->CallIntMethod(javaObject, javaGetBannerHeight);
}

// Check for banner is shown
bool LUNAAndroidAdsService::IsBannerShown()
{
	return jni::Env()->CallBooleanMethod(javaObject, javaIsBannerShown);
}

// Show banner
void LUNAAndroidAdsService::ShowBanner(const std::string& location)
{
	jni::Env()->CallVoidMethod(javaObject, javaShowBanner, jni::ToJString(location).j_str());
}

// Hide banner
void LUNAAndroidAdsService::HideBanner()
{
	jni::Env()->CallVoidMethod(javaObject, javaHideBanner);
}

// Check for interstitial is downloaded ready to showing
bool LUNAAndroidAdsService::IsInterstitialReady()
{
	return jni::Env()->CallBooleanMethod(javaObject, javaIsInterstitialReady);
}

// Cache interstitial
void LUNAAndroidAdsService::CacheInterstitial(const std::string& location)
{
	jni::Env()->CallVoidMethod(javaObject, javaCacheInterstitial, jni::ToJString(location).j_str());
}

// Show interstitial
void LUNAAndroidAdsService::ShowInterstitial(const std::string& location)
{
	jni::Env()->CallVoidMethod(javaObject, javaShowInterstitial, jni::ToJString(location).j_str());
}

// Check for video is downloaded ready to showing
bool LUNAAndroidAdsService::IsRewardedVideoReady()
{
	return jni::Env()->CallBooleanMethod(javaObject, javaIsRewardedVideoReady);
}

// Cache rewarded video
void LUNAAndroidAdsService::CacheRewardedVideo(const std::string& location)
{
	jni::Env()->CallVoidMethod(javaObject, javaCacheRewardedVideo, jni::ToJString(location).j_str());
}

// Show rewarded video
void LUNAAndroidAdsService::ShowRewardedVideo(const std::string& location)
{
	LUNAEngine::Shared()->OnPause();
	LUNAEngine::Shared()->EnablePauseHandling(false); // Protect game from resuming by operating system

	jni::Env()->CallVoidMethod(javaObject, javaShowRewardedVideo, jni::ToJString(location).j_str());
}


// Load service instance by name
std::shared_ptr<LUNAAdsService> LUNAAndroidAds::LoadService(const std::string& name)
{
	auto service = std::make_shared<LUNAAndroidAdsService>(name);

	if(!service) return nullptr;

	return service;
}


//--------------------
// Callbacks from Java
//-------------------- 
LUNA_JNI_FUNC_PACKAGE(void, services_api, LunaAdsSerivce, onInterstitialClosed)(JNIEnv* env, jmethodID method)
{
	LUNAEngine::SharedServices()->GetAds()->OnInterstitialClosed();
}

LUNA_JNI_FUNC_PACKAGE(void, services_api, LunaAdsSerivce, onRewardedVideoSuccess)(JNIEnv* env, jmethodID method)
{
	LUNAEngine::Shared()->EnablePauseHandling(true);
	LUNAEngine::Shared()->OnResume();
	LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoSuccess();
}

LUNA_JNI_FUNC_PACKAGE(void, services_api, LunaAdsSerivce, onRewardedVideoFail)(JNIEnv* env, jmethodID method)
{
	LUNAEngine::Shared()->EnablePauseHandling(true);
	LUNAEngine::Shared()->OnResume();
	LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoFail();
}

LUNA_JNI_FUNC_PACKAGE(void, services_api, LunaAdsSerivce, onRewardedVideoError)(JNIEnv* env, jmethodID method)
{
	LUNAEngine::SharedPlatformUtils()->MessageDialog("Error", "Cannot load rewarded video",
		[]()
		{
			LUNAEngine::Shared()->EnablePauseHandling(true);
			LUNAEngine::Shared()->OnResume();
			LUNAEngine::SharedServices()->GetAds()->OnRewardedVideoFail();
		});
}