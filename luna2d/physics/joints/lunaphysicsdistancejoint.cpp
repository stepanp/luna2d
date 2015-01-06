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

#include "lunaphysicsdistancejoint.h"

using namespace luna2d;

LUNAPhysicsDistanceJoint::LUNAPhysicsDistanceJoint(LUNAPhysicsBody* body1, LUNAPhysicsBody* body2, float distance,
	float anchor1X, float anchor1Y, float anchor2X, float anchor2Y, bool collideConnected)
{
	if(!body1 || !body2)
	{
		LUNA_LOGE("Attempt to create physics joint for invalid physics body");
		return;
	}

	b2DistanceJointDef def;

	def.bodyA = body1->GetB2Body();
	def.bodyB = body2->GetB2Body();
	def.localAnchorA = b2Vec2(LUNAPhysicsUtils::PixelsToMeters(anchor1X), LUNAPhysicsUtils::PixelsToMeters(anchor1Y));
	def.localAnchorB = b2Vec2(LUNAPhysicsUtils::PixelsToMeters(anchor2X), LUNAPhysicsUtils::PixelsToMeters(anchor2Y));
	def.length = LUNAPhysicsUtils::PixelsToMeters(distance);
	def.collideConnected = collideConnected;

	world = body1->GetWorld()->GetB2World();
	joint = world->CreateJoint(&def);
}
