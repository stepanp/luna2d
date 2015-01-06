//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include "lunaphysicsworld.h"
#include "lunaphysicsmodule.h"
#include "lunaengine.h"

using namespace luna2d;

LUNAPhysicsWorld::LUNAPhysicsWorld(float gravityX, float gravityY) :
	world(new b2World(b2Vec2(gravityX, gravityY))),
	debugRenderer(new LUNAPhysicsDebugRenderer()),
	acuumulatedDelta(0.0f)
{
	world->SetDebugDraw(debugRenderer);
}

LUNAPhysicsWorld::~LUNAPhysicsWorld()
{
	delete debugRenderer;
	delete world;
}

b2World* LUNAPhysicsWorld::GetB2World()
{
	return world;
}

float LUNAPhysicsWorld::GetGravityX()
{
	return LUNAPhysicsUtils::MetersToPixels(world->GetGravity().x);
}

float LUNAPhysicsWorld::GetGravityY()
{
	return LUNAPhysicsUtils::MetersToPixels(world->GetGravity().y);
}

void LUNAPhysicsWorld::SetGravity(float gravityX, float gravityY)
{
	float metersX = LUNAPhysicsUtils::PixelsToMeters(gravityX);
	float metersY = LUNAPhysicsUtils::PixelsToMeters(gravityY);
	world->SetGravity(b2Vec2(metersX, metersY));
}

void LUNAPhysicsWorld::OnUpdate(float deltaTime)
{
	acuumulatedDelta += deltaTime;

	// Update Box2D world with fixed time step
	while(acuumulatedDelta > deltaTime)
	{
		acuumulatedDelta -= BOX2D_FIXED_DELTA;
		world->Step(BOX2D_FIXED_DELTA, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
	}
}

void LUNAPhysicsWorld::OnDebugRender()
{
	world->DrawDebugData();
}
