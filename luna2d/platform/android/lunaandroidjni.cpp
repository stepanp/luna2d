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

#include "lunaandroidjni.h"
#include <android/log.h>

using namespace luna2d;
using namespace luna2d::jni;

// Saved global pointer to JNIEnv
JavaVM* luna2d::jni::javaVM = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* javaVM, void*)
{
	luna2d::jni::javaVM = javaVM; // Save global javaVM pointer

	return JNI_VERSION_1_4;
}

//-------------------------
// Scoped holder for JNIEnv 
//-------------------------
Env::Env()
{
	jint result = javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);

	// Attach JNIEnv to current thread if needed
	if(result == JNI_EDETACHED)
	{
		javaVM->AttachCurrentThread(&env, nullptr);
		attached = true;
	}
}

Env::Env(Env&& scoped) :
	env(scoped.env),
	attached(scoped.attached)
{
	scoped.env = nullptr;
	scoped.attached = false;
}

Env::~Env()
{
	if(env && attached) javaVM->DetachCurrentThread();
}

JNIEnv* Env::operator->() const
{
	return env;
};


//-------------------------
// Scope holder for jstring 
//-------------------------
JString::JString(const std::string& str) : str(Env()->NewStringUTF(str.c_str())) {}

JString::JString(jstring str) : str(str) {}

JString::JString(JString&& scoped) : str(scoped.str)
{
	scoped.str = nullptr;
}

JString::~JString()
{
	if(str)
	{
		Env()->DeleteLocalRef(str);
		str = nullptr;
	}
}

jstring JString::j_str()
{
	return str;
}


//-------------------------------------
// Cast between jstring and std::string 
//-------------------------------------
std::string luna2d::jni::FromJString(jstring javaStr)
{
	Env env;
	const char* str = env->GetStringUTFChars(javaStr, 0);
	std::string ret(str);
	env->ReleaseStringUTFChars(javaStr, str);
	return std::move(ret);
}

std::string luna2d::jni::FromJString(jobject javaStr)
{
	return FromJString(static_cast<jstring>(javaStr));
}

JString luna2d::jni::ToJString(const std::string& str)
{
	return std::move(JString(str));
}
