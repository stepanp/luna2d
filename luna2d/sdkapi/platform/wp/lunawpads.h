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

#include "lunaads.h"
#include <windows.h>

namespace luna2d{

public delegate Platform::String^ GetNameEvent();
public delegate bool IsVideoSupportedEvent();
public delegate bool IsVideoReadyEvent();
public delegate void ShowVideoEvent();
public delegate void CallbackEvent();

class LUNAWpAdsProxy : public LUNAAdsSdk
{
public:
	LUNAWpAdsProxy(Windows::UI::Core::CoreDispatcher^ dispatcher);

private:
	Windows::UI::Core::CoreDispatcher^ dispatcher;
	GetNameEvent^ getNameEvent;
	IsVideoSupportedEvent^ videoSupportedEvent;
	IsVideoReadyEvent^ videoReadyEvent;
	ShowVideoEvent^ showVideoEvent;

	std::function<void()> onSuccess, onFail;

public:
	void SetDelegates(
		GetNameEvent^ getName,
		IsVideoSupportedEvent^ videoSupported,
		IsVideoReadyEvent^ videoReady,
		ShowVideoEvent^ showVideo);

	CallbackEvent^ GetSuccessDelegate();
	CallbackEvent^ GetFailDelegate();

	virtual std::string GetName();
	virtual bool IsVideoSupported();
	virtual bool IsVideoReady();
	virtual void ShowVideo();
	virtual void SetVideoCallbacks(
		const std::function<void()>& onSuccess,
		const std::function<void()>& onFail
	);
};

}
