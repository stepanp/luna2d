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

#include "lunaphysicsedgeshape.h"

using namespace luna2d;

LUNAPhysicsEdgeShape::LUNAPhysicsEdgeShape(float x1, float y1, float x2, float y2)
{
	b2EdgeShape* edgeShape = new b2EdgeShape();

	float metersX1 = LUNAPhysicsUtils::PixelsToMeters(x1);
	float metersY1 = LUNAPhysicsUtils::PixelsToMeters(y1);
	float metersX2 = LUNAPhysicsUtils::PixelsToMeters(x2);
	float metersY2 = LUNAPhysicsUtils::PixelsToMeters(y2);

	edgeShape->Set(b2Vec2(metersX1, metersY1), b2Vec2(metersX2, metersY2));

	shape = edgeShape;
}
