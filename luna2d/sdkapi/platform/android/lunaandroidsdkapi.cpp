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
#include "lunaandroidjni.h"
#include "lunaandroidsdkstore.h"
#include "lunaconfig.h"
#include "lunalog.h"
#include <string>

using namespace luna2d;

// Helper for load and create sdk object
template<typename SdkClass>
static std::shared_ptr<SdkClass> CreateSdkObject(const std::string& classpath)
{
	// Convert classpath like "com.package.Class" 
	// to internal JNI format like "com/package/Class"
	auto jniClasspath = classpath;
	std::replace(jniClasspath.begin(), jniClasspath.end(), '.', '/');

	auto sdkObj = std::make_shared<SdkClass>(jniClasspath);
	if(!sdkObj->IsLoaded())
	{
		LUNA_LOGE("Cannot load SDK module. Java class \"%s\" not found", classpath.c_str());
		return nullptr;
	}

	return sdkObj;
}

static std::string ParseModuleType(const std::string& fullName)
{
	size_t dotPos = fullName.find("-");
	if(dotPos == std::string::npos) return "";
	return fullName.substr(0, dotPos);
}

static std::string ParseClasspath(const std::string& fullName)
{
	size_t dotPos = fullName.find("-");
	if(dotPos == std::string::npos) return "";
	return fullName.substr(dotPos + 1);
}

LUNAAndroidSdkApi::LUNAAndroidSdkApi()
{

}

void LUNAAndroidSdkApi::LoadSdkModules()
{
	auto config = LUNAEngine::Shared()->GetConfig();
	auto sdkModules = config->GetCustomValues()["sdkmodules-classpath"].array_items();

	for(const auto& item : sdkModules)
	{
		auto type = ParseModuleType(item.string_value());
		auto classpath = ParseClasspath(item.string_value());

		if(type == "store")
		{
			if((store = CreateSdkObject<LUNAAndroidSdkStore>(classpath))) BindStore();
		}

		else LUNA_LOGE("Unsupported SDK module type: \"%s\"", type.c_str());
	}
}

// Realization of "LunaBaseSdk.getConfigValue" Java-method
// SEE "com.stepanp.luna2d.sdkapi.LunaBaseSdk"
LUNA_JNI_FUNC_PACKAGE(jstring, sdkapi, LunaBaseSdk, getConfigValue)(JNIEnv* env, jmethodID method, jstring nameSpace, jstring name)
{
	return nameSpace;//jni::ToJString("getConfigValue").j_str();
}