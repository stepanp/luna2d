//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

#include "lunaanimation.h"

using namespace luna2d;

LUNAAnimation::LUNAAnimation(const FramesList& frames, float frameTime)
{
	SetFrames(frames);
	SetFrameTime(frameTime);
}

int LUNAAnimation::GetFramesCount()
{
	return frames.size();
}

const LUNAAnimation::FramesList&LUNAAnimation::GetFrames()
{
	return frames;
}

void LUNAAnimation::SetFrames(const FramesList& frames)
{
	if(frames.empty()) LUNA_RETURN_ERR("Attempt to set empty frames list to animation");

	// Animation sizes selecting by first frame sizes
	auto firstFrame = frames[0];
	if(!InitFromRegion(firstFrame)) LUNA_RETURN_ERR("Attempt to set frames list with invalid frame to animation");

	this->frames = frames;
}

float LUNAAnimation::GetFrameTime(float frameTime)
{
	return frameTime;
}

void LUNAAnimation::SetFrameTime(float frameTime)
{
	this->frameTime = frameTime;
	this->time = 0;
}

int LUNAAnimation::GetCurFrame()
{
	if(frames.empty()) return 0;
	float totalTime = frameTime * frames.size();
	return std::floor((time / totalTime) * frames.size());
}

void LUNAAnimation::SetCurFrame(int frameIndex)
{
	if(frameIndex < 0 || frameIndex >= frames.size()) LUNA_RETURN_ERR("Frame index \"%d\" is out of range", frameIndex);
	time = frameIndex * frameTime;
}

bool LUNAAnimation::IsLoop()
{
	return loop;
}

void LUNAAnimation::SetLoop(bool loop)
{
	this->loop = loop;
}

bool LUNAAnimation::IsRunning()
{
	return running;
}

// Start or resume animation
void LUNAAnimation::Start()
{
	running = true;
}

// Stop animation without reset time
void LUNAAnimation::Pause()
{
	running = false;
}

void LUNAAnimation::Stop()
{
	running = false;
	time = 0;
}

void LUNAAnimation::Update(float deltaTime)
{
	if(frames.empty()) LUNA_RETURN_ERR("Attempt to update invalid animation");

	if(running) time += deltaTime;
	if(time >= frameTime * frames.size())
	{
		time = 0;
		if(!loop) running = false;
	}

	auto curFrame = frames[GetCurFrame()];
	if(curFrame.expired()) LUNA_RETURN_ERR("Invalid frame in animation");

	auto frameRegion = curFrame.lock();
	material.texture = frameRegion->GetTexture();
	u1 = frameRegion->GetU1();
	v1 = frameRegion->GetV1();
	u2 = frameRegion->GetU2();
	v2 = frameRegion->GetV2();
}
