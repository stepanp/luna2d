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

#include <Box2D.h>
#include "lunacolor.h"

namespace luna2d{

//-------------------------
// Debug renderer for Box2D
//-------------------------
class LUNAPhysicsDebugRenderer : public b2Draw
{
public:
	LUNAPhysicsDebugRenderer();

public:
	void DrawPolygon(const b2Vec2* vertexes, int32 vertexCount, const b2Color& b2dColor);
	void DrawSolidPolygon(const b2Vec2* vertexes, int32 vertexCount, const b2Color& b2dColor);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& b2dColor);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& b2dColor);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& b2dColor);
	void DrawTransform(const b2Transform& xf);
};

}
