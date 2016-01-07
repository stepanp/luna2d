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

#include "lunaandroidprefs.h"

using namespace luna2d;

LUNAAndroidPrefs::LUNAAndroidPrefs()
{
	jni::Env env;

	// Get ref to java wrapper class
	jclass localRef = env->FindClass("com/stepanp/luna2d/LunaPrefs");
	javaPrefs = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
	env->DeleteLocalRef(localRef);

	// Get java wrapper method ids
	javaGetString = env->GetStaticMethodID(javaPrefs, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
	javaGetInt= env->GetStaticMethodID(javaPrefs, "getInt", "(Ljava/lang/String;)I");
	javaGetFloat= env->GetStaticMethodID(javaPrefs, "getFloat", "(Ljava/lang/String;)F");
	javaGetBool= env->GetStaticMethodID(javaPrefs, "getBool", "(Ljava/lang/String;)Z");
	javaSetString = env->GetStaticMethodID(javaPrefs, "setString", "(Ljava/lang/String;Ljava/lang/String;)V");
	javaSetInt = env->GetStaticMethodID(javaPrefs, "setInt", "(Ljava/lang/String;I)V");
	javaSetFloat = env->GetStaticMethodID(javaPrefs, "setFloat", "(Ljava/lang/String;F)V");
	javaSetBool = env->GetStaticMethodID(javaPrefs, "setBool", "(Ljava/lang/String;Z)V");
	javaHasValue = env->GetStaticMethodID(javaPrefs, "hasValue", "(Ljava/lang/String;)Z");
	javaRemoveValue = env->GetStaticMethodID(javaPrefs, "removeValue", "(Ljava/lang/String;)V");
	javaClear = env->GetStaticMethodID(javaPrefs, "clear", "()V");
}

// Get string value from preferences
std::string LUNAAndroidPrefs::GetString(const std::string& name)
{
	return jni::FromJString(jni::Env()->CallStaticObjectMethod(javaPrefs, javaGetString, jni::ToJString(name).j_str()));
}

// Get int value from preferences
int LUNAAndroidPrefs::GetInt(const std::string& name)
{
	return jni::Env()->CallStaticIntMethod(javaPrefs, javaGetInt, jni::ToJString(name).j_str());
}

// Get float value from preferences
float LUNAAndroidPrefs::GetFloat(const std::string& name)
{
	return jni::Env()->CallStaticFloatMethod(javaPrefs, javaGetFloat, jni::ToJString(name).j_str());
}

// Get bool value from preferences
bool LUNAAndroidPrefs::GetBool(const std::string& name)
{
	return jni::Env()->CallStaticBooleanMethod(javaPrefs, javaGetBool, jni::ToJString(name).j_str());
}

// Set string value to preferences
void LUNAAndroidPrefs::SetString(const std::string& name, const std::string& value)
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaSetString, 
		jni::ToJString(name).j_str(), jni::ToJString(value).j_str());
}

// Set int value to preferences
void LUNAAndroidPrefs::SetInt(const std::string& name, int value)
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaSetInt, jni::ToJString(name).j_str(), value);
}

// Set float value to preferences
void LUNAAndroidPrefs::SetFloat(const std::string& name, float value)
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaSetFloat, jni::ToJString(name).j_str(), value);
}

// Set bool value to preferences
void LUNAAndroidPrefs::SetBool(const std::string& name, bool value)
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaSetBool, jni::ToJString(name).j_str(), value);
}

// Check for existing value
bool LUNAAndroidPrefs::HasValue(const std::string &name)
{
	return jni::Env()->CallStaticBooleanMethod(javaPrefs, javaHasValue, jni::ToJString(name).j_str());
}

// Remove valuee from preferences
void LUNAAndroidPrefs::RemoveValue(const std::string& name)
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaRemoveValue, jni::ToJString(name).j_str());
}

// Remove all values from preferences
void LUNAAndroidPrefs::Clear()
{
	jni::Env()->CallStaticVoidMethod(javaPrefs, javaClear);
}
