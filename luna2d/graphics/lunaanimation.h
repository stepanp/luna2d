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

#pragma once

#include "lunasprite.h"

namespace luna2d{

//----------------
// Frame animation
//----------------
class LUNAAnimation : public LUNASprite
{
	LUNA_USERDATA_DERIVED(LUNASprite, LUNAAnimation)

	typedef std::vector<std::weak_ptr<LUNATextureRegion>> FramesList;

public:
	LUNAAnimation(const FramesList& frames, float frameTime);

protected:
	FramesList frames;
	float frameTime = 0;
	float time = 0;
	bool loop = false;
	bool running = false;

public:
	int GetFramesCount();
	const FramesList& GetFrames();
	void SetFrames(const FramesList& frames);
	float GetFrameTime(float frameTime);
	void SetFrameTime(float frameTime);
	int GetCurFrame();
	void SetCurFrame(int frameIndex);
	bool IsLoop();
	void SetLoop(bool loop);
	bool IsRunning();
	void Start(); // Start or resume animation
	void Pause(); // Stop animation without reset time
	void Stop(); // Stop animation
	void Update(float deltaTime);
};

}
