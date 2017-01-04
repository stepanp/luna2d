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

#include "lunarenderer.h"

namespace luna2d{

const float MAX_DELTA = 1.0f / 10.0f; // 10 FPS
const float TARGET_DELTA_TIME = 1.0f / 60.0f;
const int MOV_AVG_PERIOD = 30;

class LUNAGraphics
{
public:
	LUNAGraphics();

private:
	LUNARenderer renderer;
	std::shared_ptr<LUNACamera> camera;

	// For calculating delta time and FPS
	double lastTime, fpsTime, deltaTime, movAvgDelta;
	int framesCount, lastFrames;
	LUNAColor backColor; // Background color
	bool paused = false;

private:
	double SmoothDeltaTime(double deltaTime);

public:
	LUNARenderer* GetRenderer();
	const std::shared_ptr<LUNACamera>& GetCamera();
	int GetFps();
	float GetDeltaTime();
	int GetRenderCalls();
	int GetRenderedVertexes();
	void ResetLastTime();
	void SetBackgroundColor(float r, float g, float b);
	bool IsPaused();
	void OnPause();
	void OnResume();
	void OnUpdate();
};

}
