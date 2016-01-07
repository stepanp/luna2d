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

#pragma once

#include <jni.h>
#include <string>

//------------------------
// JNI helpers for Android
//------------------------

// Hide "ugly" JNI function name
#define LUNA_JNI_FUNC(retType, cls, name) extern "C" JNIEXPORT retType JNICALL Java_com_stepanp_luna2d_##cls##_##name

namespace luna2d{ namespace jni{

// Saved global pointer to JNIEnv
extern JavaVM* javaVM;

//----------------------------------------------------------------------
// Scoped holder for JNIEnv
// Automatically calls AttachCurrentThread/DetachCurrentThread if needed
//----------------------------------------------------------------------
class Env
{
public:
	Env();
	Env(Env&& scoped);
	~Env();

private:
	JNIEnv* env = nullptr;
	bool attached = false;

public:
	JNIEnv* operator->() const;

// Deny copying
private:
	Env(const Env&) = delete;
	const Env& operator=(const Env&) = delete;
};


//----------------------------------------------------------
// Scope holder for jstring
// Automatically delete local ref to jstring after destroyng
//----------------------------------------------------------
class JString
{
public:
	JString(const std::string& str);
	JString(jstring str);
	JString(JString&& scoped);
	~JString();

private:
	jstring str;

public:
	jstring j_str();

// Deny copying
private:
	JString(const JString&) = delete;
	const JString& operator=(const JString&) = delete;
};


//-------------------------------------
// Cast between jstring and std::string 
//-------------------------------------
std::string FromJString(jstring javaStr);
std::string FromJString(jobject javaStr);
JString ToJString(const std::string& str);


}}
