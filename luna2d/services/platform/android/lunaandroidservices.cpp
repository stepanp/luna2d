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

#include "lunaandroidservices.h"
#include "lunaandroidads.h"
#include "lunaandroidsharing.h"
#include "lunaandroidstore.h"
#include "lunaandroidleaderboards.h"
#include "lunaconfig.h"

using namespace luna2d;

void LUNAAndroidServices::LoadServices()
{
	ads = std::make_shared<LUNAAndroidAds>();
	sharing = std::make_shared<LUNAAndroidSharing>();
	store = std::make_shared<LUNAAndroidStore>();
	leaderboards = std::make_shared<LUNAAndroidLeaderboards>();

	ads->LoadServices();
}


//------------------------------------------------------
// Realizations of "LunaServices.getConfig*" Java-metods
// SEE "com.stepanp.luna2d.services.LunaServices"
//------------------------------------------------------
LUNA_JNI_FUNC_PACKAGE(bool, services, LunaServices, hasConfigValue)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return !LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].is_null();
}

LUNA_JNI_FUNC_PACKAGE(jstring, services, LunaServices, getConfigString)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	auto string = LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].string_value();

	return jni::Env()->NewStringUTF(string.c_str());
}

LUNA_JNI_FUNC_PACKAGE(int, services, LunaServices, getConfigInt)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(float, services, LunaServices, getConfigFloat)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(bool, services, LunaServices, getConfigBool)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].bool_value();
}
