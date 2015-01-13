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

#pragma once

#include "lunalua.h"
#include "lunaphysicsdebugrenderer.h"
#include "lunaphysicsutils.h"
#include <Box2D.h>

namespace luna2d{

const float BOX2D_VELOCITY_ITERATIONS = 10;
const float BOX2D_POSITION_ITERATIONS = 8;
const float BOX2D_FIXED_DELTA = 1 / 60.0f;

class LUNAPhysicsWorld
{
	LUNA_USERDATA(LUNAPhysicsWorld)

public:
	LUNAPhysicsWorld(float gravityX, float gravityY);
	~LUNAPhysicsWorld();

private:
	b2World* world;
	LUNAPhysicsDebugRenderer* debugRenderer;
	float acuumulatedDelta;

public:
	b2World* GetB2World();
	float GetGravityX();
	float GetGravityY();
	void SetGravity(float gravityX, float gravityY);
	void OnUpdate(float deltaTime);
	void OnDebugRender();
};

}
