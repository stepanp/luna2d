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

#include "lunaandroidnotifications.h"
#include "lunalog.h"
#include "lunamacro.h"

using namespace luna2d;

LUNAAndroidNotifications::LUNAAndroidNotifications() : LUNANotifications()
{
	if(!IsEnabled()) return;

	jni::Env env;

	// Get ref to java wrapper class
	jclass localRef = env->FindClass("com/stepanp/luna2d/services/LunaNotifications");
	javaNotifications = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
	env->DeleteLocalRef(localRef);

	// Get java wrapper method ids
	javaSchedule = env->GetStaticMethodID(javaNotifications, "schedule", "(Ljava/lang/String;I)V");
	javaCancel = env->GetStaticMethodID(javaNotifications, "cancel", "()V");
}

// Schedule local push notification
void LUNAAndroidNotifications::Schedule(const std::string& message, int secondsFromNow)
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());
	if(secondsFromNow <= 0) LUNA_RETURN_ERR(NOTIFICATIONS_SECONDS_ERR.c_str());

	jni::Env()->CallStaticVoidMethod(javaNotifications, javaSchedule, jni::ToJString(message).j_str(), secondsFromNow);
}

// Cancel scheduled notifications
void LUNAAndroidNotifications::Cancel()
{
	if(!IsEnabled()) LUNA_RETURN_ERR(NOTIFICATIONS_DISABLED_ERR.c_str());

	jni::Env()->CallStaticVoidMethod(javaNotifications, javaCancel);
}
