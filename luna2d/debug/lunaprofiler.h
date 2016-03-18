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

#include "lunaengine.h"
#include "lunadebug.h"

namespace luna2d{

typedef std::chrono::duration<float> Seconds;

class LUNAProfiler
{
public:
	void ProfileAsset(Seconds seconds, const std::string& tag);
};

#define LUNA_PROFILE_ASSET_BEGIN() auto _profiler_start = std::chrono::system_clock::now()
#define LUNA_PROFILE_ASSET_END(tag) \
	auto _profiler_end = std::chrono::system_clock::now(); \
	auto _profiler_elapsed = std::chrono::duration_cast<Seconds>(_profiler_end - _profiler_start); \
	LUNAEngine::SharedDebug()->GetProfiler()->ProfileAsset(_profiler_elapsed, tag)
}
