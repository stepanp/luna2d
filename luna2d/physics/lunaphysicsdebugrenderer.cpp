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

#include "lunaphysicsdebugrenderer.h"
#include "lunaphysicsutils.h"
#include "lunaengine.h"
#include "lunagraphics.h"
#include "lunalog.h"
#include "lunamath.h"

using namespace luna2d;

// Conver b2Color to LUNAColor
LUNAColor FromB2Color(const b2Color& color, float alpha = 1.0f)
{
	return LUNAColor::RgbFloat(color.r, color.b, color.g, alpha);
}

LUNAPhysicsDebugRenderer::LUNAPhysicsDebugRenderer()
{
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit; // Render shapes
	flags += b2Draw::e_jointBit; // Render joint connections
	flags += b2Draw::e_aabbBit; // Render AABB
	flags += b2Draw::e_pairBit; // Render broad-phase pairs
	flags += b2Draw::e_centerOfMassBit; // Render center of mass
	SetFlags(flags);
}

void LUNAPhysicsDebugRenderer::DrawPolygon(const b2Vec2* vertexes, int32 vertexCount, const b2Color& b2dColor)
{
	/*LUNAColor color = FromB2Color(b2dColor, 0.5f);
	for(int i = 0; i < vertexCount - 1; i++)
	{
		LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(
			vertexes[i].x, vertexes[i].y, vertexes[i + 1].x, vertexes[i + 1].y,
			color);
	}
	LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(
		vertexes[vertexCount - 1].x, vertexes[vertexCount - 1].y, vertexes[0].x, vertexes[0].y,
		color);*/
}

void LUNAPhysicsDebugRenderer::DrawSolidPolygon(const b2Vec2* vertexes, int32 vertexCount, const b2Color& b2dColor)
{
	LUNAColor color = FromB2Color(b2dColor);
	for(int i = 0; i < vertexCount - 1; i++)
	{
		float x1 = LUNAPhysicsUtils::MetersToPixels(vertexes[i].x);
		float y1 = LUNAPhysicsUtils::MetersToPixels(vertexes[i].y);
		float x2 = LUNAPhysicsUtils::MetersToPixels(vertexes[i + 1].x);
		float y2 = LUNAPhysicsUtils::MetersToPixels(vertexes[i + 1].y);

		LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(x1, y1, x2, y2, color);
	}
	LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(
		LUNAPhysicsUtils::MetersToPixels(vertexes[vertexCount - 1].x),
		LUNAPhysicsUtils::MetersToPixels(vertexes[vertexCount - 1].y),
		LUNAPhysicsUtils::MetersToPixels(vertexes[0].x),
		LUNAPhysicsUtils::MetersToPixels(vertexes[0].y),
		color);
}

void LUNAPhysicsDebugRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& b2dColor)
{
	LUNA_LOG("DrawCircle");
}

void LUNAPhysicsDebugRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& b2dColor)
{
	LUNAColor color = FromB2Color(b2dColor);

	for(int angle = 0; angle < 360; angle += 10)
	{
		float angle1 = math::DegreesToRadians(angle);
		float angle2 = math::DegreesToRadians(angle + 10);
		float x1 = LUNAPhysicsUtils::MetersToPixels(center.x + radius * std::cos(angle1));
		float y1 = LUNAPhysicsUtils::MetersToPixels(center.y + radius * std::sin(angle1));
		float x2 = LUNAPhysicsUtils::MetersToPixels(center.x + radius * std::cos(angle2));
		float y2 = LUNAPhysicsUtils::MetersToPixels(center.y + radius * std::sin(angle2));

		LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(x1, y1, x2, y2, color);
	}
}

void LUNAPhysicsDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& b2dColor)
{
	float x1 = LUNAPhysicsUtils::MetersToPixels(p1.x);
	float y1 = LUNAPhysicsUtils::MetersToPixels(p1.y);
	float x2 = LUNAPhysicsUtils::MetersToPixels(p2.x);
	float y2 = LUNAPhysicsUtils::MetersToPixels(p2.y);

	LUNAEngine::SharedGraphics()->GetRenderer()->RenderLine(x1, y1, x2, y2, FromB2Color(b2dColor));
}

void LUNAPhysicsDebugRenderer::DrawTransform(const b2Transform& xf)
{
}
