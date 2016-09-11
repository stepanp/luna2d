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

#include "lunaandroidutils.h"
#include "lunalog.h"

using namespace luna2d;

LUNAAndroidUtils::LUNAAndroidUtils()
{
	jni::Env env;

	// Get ref to java wrapper class
	jclass localRef = env->FindClass("com/stepanp/luna2d/LunaUtils");
	javaUtils = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
	env->DeleteLocalRef(localRef);

	// Get java wrapper method ids
	javaGetSystemLocale = env->GetStaticMethodID(javaUtils, "getSystemLocale", "()Ljava/lang/String;");
	javaOpenUrl = env->GetStaticMethodID(javaUtils, "openUrl", "(Ljava/lang/String;)V");
	javaMessageDialog = env->GetStaticMethodID(javaUtils, "messageDialog", "(Ljava/lang/String;Ljava/lang/String;)V");
	javaConfirmDialog = env->GetStaticMethodID(javaUtils, "confirmDialog", "(Ljava/lang/String;Ljava/lang/String;)V");
}

std::function<void()> LUNAAndroidUtils::PopMessageCallback()
{
	auto top = messageCallbacks.top();
	messageCallbacks.pop();
	return top;
}

std::function<void(bool)> LUNAAndroidUtils::PopConfirmCallback()
{
	auto top = confirmCallbacks.top();
	confirmCallbacks.pop();
	return top;
}

// Get system locale in "xx_XX" format
// Where "xx" is ISO-639 language code, and "XX" is ISO-3166 country code
std::string LUNAAndroidUtils::GetSystemLocale()
{
	return jni::FromJString(jni::Env()->CallStaticObjectMethod(javaUtils, javaGetSystemLocale));
}

// Open given url in system browser
void LUNAAndroidUtils::OpenUrl(const std::string& url)
{
	jni::Env()->CallStaticVoidMethod(javaUtils, javaOpenUrl, jni::ToJString(url).j_str());
}

// Show native dialog with "Ok" button
// "onClose" calls when dialog closed
void LUNAAndroidUtils::MessageDialog(const std::string& title, const std::string& message,
	const std::function<void()>& onClose)
{
	messageCallbacks.push(onClose);

	jni::Env()->CallStaticVoidMethod(javaUtils, javaMessageDialog, 
		jni::ToJString(title).j_str(), jni::ToJString(message).j_str());
}

// Show native dialog with "Yes" and "No" buttons
// "onClose" calls with "true" when "Yes" button pressed, and with "false" otherwise
void LUNAAndroidUtils::ConfirmDialog(const std::string& title, const std::string& message,
	const std::function<void(bool)>& onClose)
{
	confirmCallbacks.push(onClose);

	jni::Env()->CallStaticVoidMethod(javaUtils, javaConfirmDialog, 
		jni::ToJString(title).j_str(), jni::ToJString(message).j_str());
}

// Show/hide loading indicator over game view
void LUNAAndroidUtils::ShowLoadingIndicator(bool show)
{
	LUNA_LOGE("Method LUNAAndroidUtils::ShowLoadingIndicator is not implemented");
}


LUNA_JNI_FUNC(void, LunaUtils, onMessageDialogClosed)(JNIEnv* env, jmethodID method)
{
	auto platformUtils = static_cast<LUNAAndroidUtils*>(LUNAEngine::SharedPlatformUtils());
	auto onClose = platformUtils->PopMessageCallback();

	if(onClose) onClose();
}

LUNA_JNI_FUNC(void, LunaUtils, onConfirmDialogClosed)(JNIEnv* env, jmethodID method, jboolean isConfirmed)
{
	auto platformUtils = static_cast<LUNAAndroidUtils*>(LUNAEngine::SharedPlatformUtils());
	auto onClose = platformUtils->PopConfirmCallback();

	if(onClose) onClose(isConfirmed);
}
