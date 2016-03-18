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

#ifdef LUNA_DEBUG

#include "lunaengine.h"
#include "lunadebug.h"

namespace luna2d{

typedef std::chrono::duration<double> Seconds;


enum class LUNAProfilerTag
{
	ASSET,
	CUSTOM
};


class LUNAProfiler
{
public:
	void Profile(LUNAProfilerTag tag, Seconds seconds, const std::string& message);

public:
	static std::chrono::system_clock::time_point assetTime, customTime;
};

}

#define _LUNA_PROFILE_BEGIN(timeVar) timeVar = std::chrono::system_clock::now()
#define _LUNA_PROFILE_END(timeVar, tag, message) \
	{ \
		auto end = std::chrono::system_clock::now(); \
		auto elapsed = std::chrono::duration_cast<Seconds>(end - timeVar); \
		LUNAEngine::SharedDebug()->GetProfiler()->Profile(tag, elapsed, message); \
		timeVar = end; \
	}

#else

#define _LUNA_PROFILE_BEGIN(timeVar)
#define _LUNA_PROFILE_END(timeVar, tag, message)

#endif

#define LUNA_PROFILE_BEGIN() _LUNA_PROFILE_BEGIN(LUNAProfiler::customTime)
#define LUNA_PROFILE_END(message) _LUNA_PROFILE_END(LUNAProfiler::customTime, LUNAProfilerTag::CUSTOM, message)
#define LUNA_PROFILE_ASSET_BEGIN() _LUNA_PROFILE_BEGIN(LUNAProfiler::assetTime)
#define LUNA_PROFILE_ASSET_END(message) _LUNA_PROFILE_END(LUNAProfiler::assetTime, LUNAProfilerTag::ASSET, message)
