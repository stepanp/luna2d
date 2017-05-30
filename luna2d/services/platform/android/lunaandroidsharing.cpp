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

#include "lunaandroidsharing.h"
#include "lunafiles.h"
#include "lunalog.h"

using namespace luna2d;

LUNAAndroidSharingService::LUNAAndroidSharingService(const std::string& javaClasspath)
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
		LUNA_LOGE("Cannot load sharing serivce. Java class \"%s\" not found", javaClasspath.c_str());
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
	javaGetName = env->GetMethodID(javaClass, "getName", "()Ljava/lang/String;");
	javaText = env->GetMethodID(javaClass, "text", "(Ljava/lang/String;)V");
	javaImage = env->GetMethodID(javaClass, "image", "(Ljava/lang/String;Ljava/lang/String;)V");

	isLoaded = true;
}

// Check for java part of sharing service was loaded successufully
bool LUNAAndroidSharingService::IsLoaded()
{
	return isLoaded;
}

// Get name of sharing service. Should be in lower case
std::string LUNAAndroidSharingService::GetName()
{
	return jni::FromJString(jni::Env()->CallObjectMethod(javaObject, javaGetName));
}

// Share given text
void LUNAAndroidSharingService::Text(const std::string& text)
{
	jni::Env()->CallVoidMethod(javaObject, javaText, jni::ToJString(text).j_str());
}

// Share given image with given text
// Image should be located in "LUNAFileLocation::CACHE"
void LUNAAndroidSharingService::Image(const std::string& imagePath, const std::string& text)
{
	jni::Env()->CallVoidMethod(javaObject, javaImage, jni::ToJString(imagePath).j_str(), jni::ToJString(text).j_str());
}


// Load service instance by name
std::shared_ptr<LUNASharingService> LUNAAndroidSharing::LoadService(const std::string& name)
{
	std::string classpath = name == "" ? "com.stepanp.luna2d.services.LunaDefaultSharing" : name;
	auto service = std::make_shared<LUNAAndroidSharingService>(classpath);

	if(!service) return nullptr;

	return service;
}
