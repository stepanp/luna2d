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

#include "lunaandroidsdkapi.h"
#include "lunaandroidsdkstore.h"
#include "lunaconfig.h"
#include "lunalog.h"

using namespace luna2d;

LUNAAndroidSdkWrapper::LUNAAndroidSdkWrapper(const std::string& javaClasspath)
{
	// Convert classpath like "com.package.Class" 
	// to internal JNI format like "com/package/Class"
	auto jniClasspath = javaClasspath;
	std::replace(jniClasspath.begin(), jniClasspath.end(), '.', '/');

	jni::Env env;

	// Get ref to java wrapper class
	jclass localClassRef = env->FindClass(jniClasspath.c_str());
	if(env->ExceptionCheck()) 
	{
		env->ExceptionClear();
		LUNA_LOGE("Cannot load SDK module. Java class \"%s\" not found", javaClasspath.c_str());
		return;
	}

	javaClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClassRef));
	env->DeleteLocalRef(localClassRef);

	// Make new object of java wrapper class
	jmethodID constructor = env->GetMethodID(javaClass, "<init>", "()V");
	jobject localObjRef = env->NewObject(javaClass, constructor);
	javaObject = reinterpret_cast<jobject>(env->NewGlobalRef(localObjRef));
	env->DeleteLocalRef(localObjRef);

	isLoaded = true;
}

LUNAAndroidSdkWrapper::~LUNAAndroidSdkWrapper()
{
	jni::Env env;
	if(javaObject) env->DeleteGlobalRef(javaObject);
}

bool LUNAAndroidSdkWrapper::IsLoaded()
{
	return isLoaded;
}


static std::string ParseModuleType(const std::string& fullName)
{
	size_t dotPos = fullName.find("-");
	if(dotPos == std::string::npos) return "";
	return fullName.substr(0, dotPos);
}

static std::string ParseModuleName(const std::string& fullName)
{
	size_t dotPos = fullName.find("-");
	if(dotPos == std::string::npos) return "";
	return fullName.substr(dotPos + 1);
}


void LUNAAndroidSdkApi::LoadModules()
{
	auto config = LUNAEngine::Shared()->GetConfig();
	auto sdkModules = config->GetCustomValues()["sdkmodules"].array_items();
	auto sdkModulesClasspath = config->GetCustomValues()["sdkmodules-classpath"].array_items();

	for(size_t i = 0; i < sdkModules.size(); i++)
	{
		const auto& module = sdkModules[i];

		LoadModule(ParseModuleType(module.string_value()), ParseModuleName(module.string_value()), 
			sdkModulesClasspath[i].string_value());
	}
}

void LUNAAndroidSdkApi::LoadModule(const std::string& type, const std::string& name, const std::string& classpath)
{
	if(type == "store")
	{
		auto storeSdk = std::make_shared<LUNAAndroidSdkStore>(name, classpath);
		if(storeSdk->IsLoaded())
		{
			store = storeSdk;
			BindStore();
		}
	}

	else LUNA_LOGE("Unsupported SDK module type: \"%s\"", type.c_str());
}


//-----------------------------------------------------
// Realizations of "LunaBaseSdk.getConfig*" Java-metods
// SEE "com.stepanp.luna2d.sdkapi.LunaBaseSdk"
//-----------------------------------------------------

LUNA_JNI_FUNC_PACKAGE(bool, sdkapi, LunaBaseSdk, hasConfigValue)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return !LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].is_null();
}

LUNA_JNI_FUNC_PACKAGE(jstring, sdkapi, LunaBaseSdk, getConfigString)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	auto string = LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].string_value();

	return jni::Env()->NewStringUTF(string.c_str());
}

LUNA_JNI_FUNC_PACKAGE(int, sdkapi, LunaBaseSdk, getConfigInt)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(float, sdkapi, LunaBaseSdk, getConfigFloat)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].number_value();
}

LUNA_JNI_FUNC_PACKAGE(bool, sdkapi, LunaBaseSdk, getConfigBool)(JNIEnv* env, jmethodID method, jstring jName)
{
	auto name = jni::FromJString(jName);
	return LUNAEngine::Shared()->GetConfig()->GetCustomValues()[name].bool_value();
}