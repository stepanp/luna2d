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

#include "lunaandroidstore.h"

using namespace luna2d;

LUNAAndroidStore::LUNAAndroidStore()
{
	jni::Env env;

	// Get ref to java wrapper class
	jclass localRef = env->FindClass("com/stepanp/luna2d/services/LunaStore");
	javaStore = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
	env->DeleteLocalRef(localRef);

	// Get java wrapper method ids
	javaGetUrl = env->GetStaticMethodID(javaStore, "getUrl", "()Ljava/lang/String;");
	javaOpenPage = env->GetStaticMethodID(javaStore, "openPage", "()V");
	javaRequestRateApp = env->GetStaticMethodID(javaStore, "requestRateApp", "()V");
}

// Get url to page of game in store
std::string LUNAAndroidStore::GetUrl()
{
	return jni::FromJString(jni::Env()->CallStaticObjectMethod(javaStore, javaGetUrl));
}

// Open page of game in store
void LUNAAndroidStore::OpenPage()
{
	jni::Env()->CallStaticVoidMethod(javaStore, javaOpenPage);
}

// Request rate app dialog
void LUNAAndroidStore::RequestRateApp()
{
	jni::Env()->CallStaticVoidMethod(javaStore, javaRequestRateApp);
}

// Show rate app dialog
void LUNAAndroidStore::ShowRateApp()
{
	
}
