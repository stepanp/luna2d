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

#include "lunalua.h"

namespace luna2d{

class LUNATimer
{
	LUNA_USERDATA(LUNATimer)

public:
	LUNATimer(float time, const LuaFunction& onFinish, bool loop);

private:
	float totalTime = 0.0f, time = 0.0f;
	LuaFunction onFinish = nil;
	bool loop = false;
	bool running = false;

public:
	bool IsLoop();
	void SetLoop(bool loop);
	float GetTotalTime();
	float GetRemainingTime();
	void SetTime(float time);
	LuaFunction GetCallback();
	void SetCallback(const LuaFunction& onFinish);
	bool IsRunning();
	void Start(); // Start or resume timer
	void Pause(); // Stop timer without reset time
	void Stop(); // Stop timer
	void Update(float deltaTime);
};

}
