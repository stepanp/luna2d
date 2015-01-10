//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#include "lunaphysicsworld.h"

namespace luna2d{

class LUNAPhysicsShape;

class LUNAPhysicsBody
{
	LUNA_USERDATA(LUNAPhysicsBody)

public:
	LUNAPhysicsBody(LUNAPhysicsWorld* world, int type);
	~LUNAPhysicsBody();

private:
	LUNAPhysicsWorld* world;
	b2Body* body;

public:
	LUNAPhysicsWorld* GetWorld();
	b2Body* GetB2Body();
	bool IsDestroyed();
	void Destroy();
	float GetX();
	float GetY();
	void SetX(float x);
	void SetY(float y);
	LuaTable GetPos();
	void SetPos(float x, float y);
	float GetAngle();
	void SetAngle(float angle);

	void ApplyForce(float forceX, float forceY, float pointX, float pointY);
	void ApplyForceToCenter(float forceX, float forceY);
	void ApplyTorque(float torque);
	void ApplyLinearImpulse(float impulseX, float impulseY, float pointX, float pointY);
	void ApplyAngularImpulse(float impulse);

	LuaTable GetPoints(LUNAPhysicsShape* shape);
};

}
