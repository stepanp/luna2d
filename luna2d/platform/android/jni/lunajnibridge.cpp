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

#include "lunaengine.h"
#include "lunaassets.h"
#include "lunagraphics.h"
#include "lunaandroidfiles.h"
#include "lunaandroidlog.h"
#include "lunaandroidutils.h"
#include "lunaandroidprefs.h"
#include "lunaandroidjni.h"
#include "lunasizes.h"

using namespace luna2d;

LUNA_JNI_FUNC(void, LunaNative, initialize)(JNIEnv* env, jclass cls, jint screenWidth, jint screenHeight,
		jstring appName, jstring apkPath, jstring appFolderPath)
{
	// Initialize file utils
	LUNAFiles* files = new LUNAAndroidFiles(jni::FromJString(apkPath), jni::FromJString(appFolderPath));

	// Initialize log
	LUNALog* log = new LUNAAndroidLog(jni::FromJString(appName));

	// Initialize platform-specific tools
	LUNAPlatformUtils* platformUtils = new LUNAAndroidUtils();

	// Initialize preferences util
	LUNAAndroidPrefs* prefs = new LUNAAndroidPrefs();

	// Initialize engine
	LUNAEngine::Shared()->Assemble(files, log, platformUtils, prefs);
	LUNAEngine::Shared()->Initialize(screenWidth, screenHeight);
}

LUNA_JNI_FUNC(void, LunaNative, reloadAssets)(JNIEnv* env, jclass cls)
{
	// Reset view port
	glViewport(0, 0, LUNAEngine::SharedSizes()->GetPhysicalScreenWidth(), LUNAEngine::SharedSizes()->GetPhysicalScreenHeight());
	
	LUNAEngine::SharedGraphics()->GetRenderer()->ReloadDefaultShader();
	LUNAEngine::SharedAssets()->ReloadAssets();
}

LUNA_JNI_FUNC(jboolean, LunaNative, isInitialized)(JNIEnv* env, jclass cls)
{
	return LUNAEngine::Shared()->IsInitialized();
}

LUNA_JNI_FUNC(void, LunaNative, mainLoop)(JNIEnv* env, jclass cls)
{
	LUNAEngine::Shared()->MainLoop();
}

LUNA_JNI_FUNC(void, LunaNative, onTouchDown)(JNIEnv* env, jclass cls, jfloat x, jfloat y, jint touchIndex)
{
	LUNAEngine::Shared()->OnTouchDown(x, y, touchIndex);
}

LUNA_JNI_FUNC(void, LunaNative, onTouchMoved)(JNIEnv* env, jclass cls, jfloat x, jfloat y, jint touchIndex)
{
	LUNAEngine::Shared()->OnTouchMoved(x, y, touchIndex);
}

LUNA_JNI_FUNC(void, LunaNative, onTouchUp)(JNIEnv* env, jclass cls, jfloat x, jfloat y, jint touchIndex)
{
	LUNAEngine::Shared()->OnTouchUp(x, y, touchIndex);
}

LUNA_JNI_FUNC(void, LunaNative, onPause)(JNIEnv* env, jclass cls)
{
	if(LUNAEngine::Shared()->IsInitialized()) LUNAEngine::Shared()->OnPause();
}

LUNA_JNI_FUNC(void, LunaNative, onResume)(JNIEnv* env, jclass cls)
{
	if(LUNAEngine::Shared()->IsInitialized()) LUNAEngine::Shared()->OnResume();
}

