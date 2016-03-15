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

#include "lunabasesdk.h"

namespace luna2d{

class LUNAAdsSdk : public LUNABaseSdk
{
public:
	virtual	~LUNAAdsSdk() {}

public:
	virtual bool IsVideoSupported() = 0;

	virtual bool IsVideoReady() = 0;

	virtual void CacheVideo() = 0;

	virtual void ShowVideo() = 0;
};


class LUNAAds
{
private:
	std::shared_ptr<LUNAAdsSdk> video;

public:
	void AddVideoSdk(const std::shared_ptr<LUNAAdsSdk>& video);

	bool IsVideoReady();

	void CacheVideo();

	void RequestVideo();

	void OnVideoCompleted(const std::string& sdkName);

	void OnVideoCanceled(const std::string& sdkName);

	void OnVideoError(const std::string& sdkName, const std::string& error);
};

}
