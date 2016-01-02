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

#include "lunaphysicsbody.h"
#include "shapes/lunaphysicsshape.h"
#include "lunamath.h"

using namespace luna2d;

LUNAPhysicsBody::LUNAPhysicsBody(std::shared_ptr<LUNAPhysicsWorld> world, int type) :
	world(world.get()),
	body(nullptr)
{
	b2BodyDef def;
	def.type = static_cast<b2BodyType>(type);
	def.userData = this;

	body = world->GetB2World()->CreateBody(&def);
}

LUNAPhysicsBody::~LUNAPhysicsBody()
{
	//if(!IsDestroyed()) Destroy();
}

LUNAPhysicsWorld *LUNAPhysicsBody::GetWorld()
{
	return world;
}

b2Body* LUNAPhysicsBody::GetB2Body()
{
	return body;
}

bool LUNAPhysicsBody::IsDestroyed()
{
	return body == nullptr;
}

void LUNAPhysicsBody::Destroy()
{
	if(IsDestroyed())
	{
		LUNA_LOGE("Body already destroyed");
		return;
	}

	world->GetB2World()->DestroyBody(body);

	world = nullptr;
	body = nullptr;
}

float LUNAPhysicsBody::GetX()
{
	return LUNAPhysicsUtils::MetersToPixels(body->GetPosition().x);
}

float LUNAPhysicsBody::GetY()
{
	return LUNAPhysicsUtils::MetersToPixels(body->GetPosition().y);
}

void LUNAPhysicsBody::SetX(float x)
{
	SetPos(x, body->GetPosition().y);
}

void LUNAPhysicsBody::SetY(float y)
{
	SetPos(body->GetPosition().x, y);
}

LuaTable LUNAPhysicsBody::GetPos()
{
	return LUNAPhysicsUtils::B2VectorToTable(body->GetPosition());
}

void LUNAPhysicsBody::SetPos(float x, float y)
{
	float metersX = LUNAPhysicsUtils::PixelsToMeters(x);
	float metersY = LUNAPhysicsUtils::PixelsToMeters(y);
	body->SetTransform(b2Vec2(metersX, metersY), body->GetAngle());
}

float LUNAPhysicsBody::GetAngle()
{
	return math::RadiansToDegrees(body->GetAngle());
}

void LUNAPhysicsBody::SetAngle(float angle)
{
	body->SetTransform(body->GetPosition(), math::DegreesToRadians(angle));
}

void LUNAPhysicsBody::ApplyForce(float forceX, float forceY, float pointX, float pointY)
{
	b2Vec2 metersPoint(LUNAPhysicsUtils::PixelsToMeters(pointX), LUNAPhysicsUtils::PixelsToMeters(pointY));
	body->ApplyForce(b2Vec2(forceX, forceY), metersPoint, true);
}

void LUNAPhysicsBody::ApplyForceToCenter(float forceX, float forceY)
{
	body->ApplyForceToCenter(b2Vec2(forceX, forceY), true);
}

void LUNAPhysicsBody::ApplyTorque(float torque)
{
	body->ApplyTorque(torque, true);
}

void LUNAPhysicsBody::ApplyLinearImpulse(float impulseX, float impulseY, float pointX, float pointY)
{
	b2Vec2 metersPoint(LUNAPhysicsUtils::PixelsToMeters(pointX), LUNAPhysicsUtils::PixelsToMeters(pointY));
	body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), metersPoint, true);
}

void LUNAPhysicsBody::ApplyAngularImpulse(float impulse)
{
	body->ApplyAngularImpulse(impulse, true);
}

LuaTable LUNAPhysicsBody::GetPoints(std::shared_ptr<LUNAPhysicsShape> shape)
{
	if(!shape || !shape->GetB2Shape()) return nil;

	b2Shape::Type type = shape->GetB2Shape()->GetType();
	if(type == b2Shape::Type::e_polygon)
	{
		LuaTable tblRet(LUNAEngine::SharedLua());
		b2PolygonShape* polygonShape = static_cast<b2PolygonShape*>(shape->GetB2Shape());

		int count = polygonShape->GetVertexCount();
		for(int i = 0; i < count; i++)
		{
			b2Vec2 vertex = body->GetWorldPoint(polygonShape->GetVertex(i));
			tblRet.SetArrayField(i + 1, LUNAPhysicsUtils::B2VectorToTable(vertex));
		}

		return std::move(tblRet);
	}

	return nil;
}
