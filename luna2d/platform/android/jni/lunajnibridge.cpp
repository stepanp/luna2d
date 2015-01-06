//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaengine.h"
#include "lunaandroidfiles.h"
#include "lunaandroidlog.h"
#include "lunaandroidutils.h"
#include "lunaandroidjni.h"

using namespace luna2d;

LUNA_JNI_FUNC(void, LunaNative, initialize)(JNIEnv* env, jclass cls, jint jscreenWidth, jint jscreenHeight,
		jstring javaAppName, jstring javaApkPath)
{
	// Initialize engine at first creating GL surface
	if(!LUNAEngine::Shared()->IsInitialized())
	{
		// Initialize file utils
		const char* apkPath = env->GetStringUTFChars(javaApkPath, 0);
		LUNAFiles* files = new LUNAAndroidFiles(apkPath);
		env->ReleaseStringUTFChars(javaApkPath, apkPath);

		// Set application name as tag for logging
		const char* appName = env->GetStringUTFChars(javaAppName, 0);
		LUNALog* log = new LUNAAndroidLog(appName);
		env->ReleaseStringUTFChars(javaAppName, appName);

		// Initialize platform-specific tools
		LUNAPlatformUtils* platformUtils = new LUNAAndroidUtils();

		// Initialize engine
		LUNAEngine::Shared()->Assemble(files, log, platformUtils);
		LUNAEngine::Shared()->Initialize(jscreenWidth, jscreenHeight);
	}

	// GL surface recreating when GL context was lost
	// In that case, we need reload some assets: textures, shaders, etc.
	else
	{
		LUNAEngine::Shared()->ReloadAssets();
	}
}

LUNA_JNI_FUNC(void, LunaNative, mainLoop)(JNIEnv* env, jclass cls)
{
	LUNAEngine::Shared()->MainLoop();
}

LUNA_JNI_FUNC(void, LunaNative, onTouchDown)(JNIEnv* env, jclass cls, jfloat x, jfloat y)
{
	LUNAEngine::Shared()->OnTouchDown(x, y);
}

LUNA_JNI_FUNC(void, LunaNative, onTouchMoved)(JNIEnv* env, jclass cls, jfloat x, jfloat y)
{
	LUNAEngine::Shared()->OnTouchMoved(x, y);
}

LUNA_JNI_FUNC(void, LunaNative, onTouchUp)(JNIEnv* env, jclass cls, jfloat x, jfloat y)
{
	LUNAEngine::Shared()->OnTouchUp(x, y);
}

