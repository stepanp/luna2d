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

#include "lunatimer.h"

using namespace luna2d;

LUNATimer::LUNATimer(float time, const LuaFunction& onFinish, bool loop)
{
	SetTime(time);
	SetCallback(onFinish);
	SetLoop(loop);
}

bool LUNATimer::IsLoop()
{
	return loop;
}

void LUNATimer::SetLoop(bool loop)
{
	this->loop = loop;
}

float LUNATimer::GetTotalTime()
{
	return totalTime;
}

float LUNATimer::GetRemainingTime()
{
	return totalTime - time;
}

void LUNATimer::SetTime(float time)
{
	Stop();

	this->totalTime = time;
	this->time = 0;
}

LuaFunction LUNATimer::GetCallback()
{
	return onFinish;
}

void LUNATimer::SetCallback(const LuaFunction& onFinish)
{
	if(!onFinish)
	{
		LUNA_LOGE("Attempt to set invalid finish callback to timer");
		return;
	}

	this->onFinish = onFinish;
}

bool LUNATimer::IsRunning()
{
	return running;
}

// Start or resume timer
void LUNATimer::Start()
{
	running = true;
}

// Stop timer without reset time
void LUNATimer::Pause()
{
	running = false;
}

// Stop timer
void LUNATimer::Stop()
{
	running = false;
	time = 0;
}

void LUNATimer::Update(float deltaTime)
{
	if(!running) return;

	time += deltaTime;
	if(time >= totalTime)
	{
		time = 0;
		if(!loop) running = false;
		if(onFinish) onFinish.CallVoid();
	}
}
