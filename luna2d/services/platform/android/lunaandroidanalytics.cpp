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

#include "lunaandroidanalytics.h"
#include "lunalog.h"
#include "lunaplatformutils.h"
#include "lunaandroidservices.h"

using namespace luna2d;

LUNAAndroidAnalyticsService::LUNAAndroidAnalyticsService(const std::string& javaClassPath)
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
		LUNA_LOGE("Cannot load analytics serivce. Java class \"%s\" not found", javaClassPath.c_str());
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
	javaClearData = env->GetMethodID(javaClass, "clearData", "()V");
	javaPutData = env->GetMethodID(javaClass, "putData", "(Ljava/lang/String;Ljava/lang/String;)V");
	javaSendData = env->GetMethodID(javaClass, "sendData", "(Ljava/lang/String;)V");

	isLoaded = true;
}

// Check for java part of ads service was loaded successufully
bool LUNAAndroidAnalyticsService::IsLoaded()
{
	return isLoaded;
}

// Send data to analytics
void LUNAAndroidAnalyticsService::Send(const std::string& event, const std::unordered_map<std::string,std::string>& data)
{
	jni::Env()->CallVoidMethod(javaObject, javaClearData);

	for(const auto& entry : data)
	{
		jni::Env()->CallVoidMethod(javaObject, javaPutData, jni::ToJString(entry.first).j_str(), jni::ToJString(entry.second).j_str());		
	}

	jni::Env()->CallVoidMethod(javaObject, javaSendData, jni::ToJString(event).j_str());
}


// Load service instance by name
std::shared_ptr<LUNAAnalyticsService> LUNAAndroidAnalytics::LoadService(const std::string& name)
{
	auto service = std::make_shared<LUNAAndroidAnalyticsService>(name);

	if(!service) return nullptr;

	return service;
}
