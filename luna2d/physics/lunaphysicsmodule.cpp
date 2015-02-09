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

#include "lunaphysicsmodule.h"
#include "lunaphysicsworld.h"
#include "lunaphysicsbody.h"
#include "lunaphysicsfixture.h"
#include "lunaphysicsutils.h"
#include "shapes/lunaphysicsshape.h"
#include "shapes/lunaphysicsrectshape.h"
#include "shapes/lunaphysicscircleshape.h"
#include "shapes/lunaphysicsedgeshape.h"
#include "joints/lunaphysicsjoint.h"
#include "joints/lunaphysicsdistancejoint.h"

using namespace luna2d;

void LUNAPhysicsModule::Load(luna2d::LuaScript* lua)
{
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblPhysics(lua);

	// Register physics module functions
	tblPhysics.SetField("setPixelsInMeter", LuaFunction(lua, &LUNAPhysicsUtils::SetPixelsInMeter));

	// Register world
	LuaClass<LUNAPhysicsWorld> clsWorld(lua);
	clsWorld.SetConstructor<float,float>();
	clsWorld.SetMethod("getGravityX", &LUNAPhysicsWorld::GetGravityX);
	clsWorld.SetMethod("getGravityY", &LUNAPhysicsWorld::GetGravityY);
	clsWorld.SetMethod("setGravity", &LUNAPhysicsWorld::SetGravity);
	clsWorld.SetMethod("update", &LUNAPhysicsWorld::OnUpdate);
	clsWorld.SetMethod("debugRender", &LUNAPhysicsWorld::OnDebugRender);
	tblPhysics.SetField("World", clsWorld);

	// Register body
	LuaClass<LUNAPhysicsBody> clsBody(lua);
	clsBody.SetConstructor<std::shared_ptr<LUNAPhysicsWorld>, int>();
	clsBody.SetMethod("destroy", &LUNAPhysicsBody::Destroy);
	clsBody.SetMethod("getX", &LUNAPhysicsBody::GetX);
	clsBody.SetMethod("getY", &LUNAPhysicsBody::GetY);
	clsBody.SetMethod("setX", &LUNAPhysicsBody::SetX);
	clsBody.SetMethod("setY", &LUNAPhysicsBody::SetY);
	clsBody.SetMethod("getPos", &LUNAPhysicsBody::GetPos);
	clsBody.SetMethod("setPos", &LUNAPhysicsBody::SetPos);
	clsBody.SetMethod("getAngle", &LUNAPhysicsBody::GetAngle);
	clsBody.SetMethod("setAngle", &LUNAPhysicsBody::SetAngle);
	clsBody.SetMethod("applyForce", &LUNAPhysicsBody::ApplyForce);
	clsBody.SetMethod("applyForceToCenter", &LUNAPhysicsBody::ApplyForceToCenter);
	clsBody.SetMethod("applyTorque", &LUNAPhysicsBody::ApplyTorque);
	clsBody.SetMethod("applyLinearImpulse", &LUNAPhysicsBody::ApplyLinearImpulse);
	clsBody.SetMethod("applyAngularImpulse", &LUNAPhysicsBody::ApplyAngularImpulse);
	clsBody.SetMethod("getPoints", &LUNAPhysicsBody::GetPoints);
	tblPhysics.SetField("Body", clsBody);

	// Register base shape
	LuaClass<LUNAPhysicsShape> clsShape(lua);

	// Register rect shape
	LuaClass<LUNAPhysicsRectShape> clsRectShape(lua);
	clsRectShape.SetConstructor<float, float>();
	tblPhysics.SetField("RectShape", clsRectShape);

	// Register circle shape
	LuaClass<LUNAPhysicsCircleShape> clsCircleShape(lua);
	clsCircleShape.SetConstructor<float>();
	tblPhysics.SetField("CircleShape", clsCircleShape);

	// Register edge shape
	LuaClass<LUNAPhysicsEdgeShape> clsEdgEShape(lua);
	clsEdgEShape.SetConstructor<float,float,float,float>();
	tblPhysics.SetField("EdgeShape", clsEdgEShape);

	// Register fixture
	LuaClass<LUNAPhysicsFixture> clsFixture(lua);
	clsFixture.SetConstructor<std::shared_ptr<LUNAPhysicsBody>,std::shared_ptr<LUNAPhysicsShape>>();
	clsFixture.SetMethod("setFilterGroup", &LUNAPhysicsFixture::SetFilterGroup);
	tblPhysics.SetField("Fixture", clsFixture);

	// Register base joint
	LuaClass<LUNAPhysicsJoint> clsJoint(lua);
	clsJoint.SetMethod("destroy", &LUNAPhysicsJoint::Destroy);
	clsJoint.SetMethod("getAnchorA", &LUNAPhysicsJoint::GetAnchorA);
	clsJoint.SetMethod("getAnchorB", &LUNAPhysicsJoint::GetAnchorB);

	// Register distance joint
	LuaClass<LUNAPhysicsDistanceJoint> clsDistanceJoint(lua);
	clsDistanceJoint.SetConstructor<std::shared_ptr<LUNAPhysicsBody>,std::shared_ptr<LUNAPhysicsBody>,
			float,float,float,float,float,bool>();
	tblPhysics.SetField("DistanceJoint", clsDistanceJoint);

	// Register constants
	tblPhysics.SetField("GRAVITY_EARTH", -9.8f);
	tblPhysics.SetField("BODY_STATIC", static_cast<int>(b2_staticBody));
	tblPhysics.SetField("BODY_KINEMATIC", static_cast<int>(b2_kinematicBody));
	tblPhysics.SetField("BODY_DYNAMIC", static_cast<int>(b2_dynamicBody));

	tblPhysics.MakeReadOnly();
	tblLuna.SetField("physics", tblPhysics);
}
