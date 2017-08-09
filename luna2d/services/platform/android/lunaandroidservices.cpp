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

#include "lunaandroidservices.h"
#include "lunaandroidads.h"
#include "lunaandroidpurchases.h"
#include "lunaandroidsharing.h"
#include "lunaandroidstore.h"
#include "lunaandroidleaderboards.h"
#include "lunaandroidnotifications.h"
#include "lunaandroidanalytics.h"
#include "lunaconfig.h"

using namespace luna2d;

void LUNAAndroidServices::LoadServices()
{
	ads = std::make_shared<LUNAAndroidAds>();
	purchases = std::make_shared<LUNAAndroidPurchases>();
	sharing = std::make_shared<LUNAAndroidSharing>();
	store = std::make_shared<LUNAAndroidStore>();
	leaderboards = std::make_shared<LUNAAndroidLeaderboards>();
	notifications = std::make_shared<LUNAAndroidNotifications>();
	analytics = std::make_shared<LUNAAndroidAnalytics>();

	ads->LoadServices();
	sharing->LoadServices();
	analytics->LoadServices();
}


//------------------------------------------------------
// Realizations of "LunaServices.getConfig*" Java-metods
// SEE "com.stepanp.luna2d.services.LunaServices"
//------------------------------------------------------
LUNA_JNI_FUNC_PACKAGE(bool, services_api, LunaServicesApi, hasConfigValue)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	return !LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].is_null();
}

LUNA_JNI_FUNC_PACKAGE(jstring, services_api, LunaServicesApi, getConfigString)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	auto string = LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].string_value();

	return jni::Env()->NewStringUTF(string.c_str());
}

LUNA_JNI_FUNC_PACKAGE(int, services_api, LunaServicesApi, getConfigInt)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(float, services_api, LunaServicesApi, getConfigFloat)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(bool, services_api, LunaServicesApi, getConfigBool)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].bool_value();
}

LUNA_JNI_FUNC_PACKAGE(jobjectArray, services_api, LunaServicesApi, getConfigStringArray)(JNIEnv* env, jclass cls, jstring jName)
{
	auto name = jni::FromJString(jName);
	auto items = LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].array_items();

	jobjectArray ret = jni::Env()->NewObjectArray(items.size(), 
		jni::Env()->FindClass("java/lang/String"), jni::Env()->NewStringUTF(""));

    for(int i = 0; i < items.size(); i++) 
    {  
        jni::Env()->SetObjectArrayElement(ret, i, jni::Env()->NewStringUTF(items[i].string_value().c_str()));  
    }  

    return ret;
}
