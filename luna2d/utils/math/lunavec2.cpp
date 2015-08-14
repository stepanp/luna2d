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

#include "lunavec2.h"

using namespace luna2d;

int Constructor(HSQUIRRELVM vm)
{
	int argsCount = sq_gettop(vm);

	// Make empty vector
	if(argsCount == 1) SqStack<glm::vec2>::Push(vm, glm::vec2());

	// Make copy of given vector
	else if(argsCount == 2) SqStack<glm::vec2>::Push(vm, SqStack<glm::vec2>::Get(vm, 2));

	// Make vector from given coordinates
	else if(argsCount == 3)
	{
		float x = SqStack<float>::Get(vm, 2);
		float y = SqStack<float>::Get(vm, 3);
		SqStack<glm::vec2>::Push(vm, glm::vec2(x, y));
	}

	else return sq_throwerror(vm, "wrong number of parameters");

	return 1;
}

// Add given vector to this vector
int Add(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<glm::vec2>::Set(vm, vec1 += vec2, 1);
	sq_push(vm, 1);
	return 1;
}

// Substract given vector from this vector
int Sub(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<glm::vec2>::Set(vm, vec1 -= vec2, 1);
	sq_push(vm, 1);
	return 1;
}

// Multiple this vector to scalar
int Scale(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	float scale = SqStack<float>::Get(vm, 2);

	SqStack<glm::vec2>::Set(vm, vec *= scale, 1);
	sq_push(vm, 1);
	return 1;
}

// Rotate this vector to given angle (in degrees)
int Rotate(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	float angle = SqStack<float>::Get(vm, 2);

	SqStack<glm::vec2>::Set(vm, glm::rotate(vec, glm::radians(angle)), 1);
	sq_push(vm, 1);
	return 1;
}

// Rotate this vector to given angle (in radians)
int Rotater(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	float angle = SqStack<float>::Get(vm, 2);

	SqStack<glm::vec2>::Set(vm, glm::rotate(vec, angle), 1);
	sq_push(vm, 1);
	return 1;
}

// Normalize this vector
int Nor(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	SqStack<glm::vec2>::Set(vm, glm::normalize(vec), 1);
	sq_push(vm, 1);
	return 1;
}

// Make perpendicular of this vector
int Perp(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	SqStack<glm::vec2>::Set(vm, glm::vec2(-vec.y, vec.x), 1);
	sq_push(vm, 1);
	return 1;
}

// Get angle of this vector (in degrees)
int Angle(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);

	if(glm::epsilonEqual(vec.x, vec.y, GLM_EPSILON)) SqStack<float>::Push(vm, 0.0f);
	else
	{
		float angle = glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vec));
		SqStack<float>::Push(vm,  glm::degrees(angle));
	}
	return 1;
}

// Get angle of this vector (in radians)
int Angler(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);

	if(glm::epsilonEqual(vec.x, vec.y, GLM_EPSILON)) SqStack<float>::Push(vm, 0.0f);
	else
	{
		float angle = glm::orientedAngle(glm::vec2(1.0f, 0.0f), glm::normalize(vec));
		SqStack<float>::Push(vm, angle);
	}
	return 1;
}

// Get lenght of this vector
int Len(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	SqStack<float>::Push(vm, glm::length(vec));
	return 1;
}

// Get squared lenght of this vector
int LenSqr(HSQUIRRELVM vm)
{
	glm::vec2 vec = SqStack<glm::vec2>::Get(vm, 1);
	SqStack<float>::Push(vm, glm::length2(vec));
	return 1;
}

// Get distance between this and given vectors
int Dist(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<float>::Push(vm, glm::distance(vec1, vec2));
	return 1;
}

// Get squared distance between this and given vectors
int DistSqr(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<float>::Push(vm, glm::distance2(vec1, vec2));
	return 1;
}

// Get cross prodict between this and given vectors
int Cross(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<float>::Push(vm, vec1.x * vec2.y - vec2.x * vec1.y);
	return 1;
}

// Get dot prodict between this and given vectors
int Dot(HSQUIRRELVM vm)
{
	glm::vec2 vec1 = SqStack<glm::vec2>::Get(vm, 1);
	glm::vec2 vec2 = SqStack<glm::vec2>::Get(vm, 2);

	SqStack<float>::Push(vm, glm::dot(vec1, vec2));
	return 1;
}

// Make copy of this vector
int Copy(HSQUIRRELVM vm)
{
	SqStack<glm::vec2>::Push(vm, SqStack<glm::vec2>::Get(vm, 1));
	return 1;
}

void luna2d::BindVec2(SqVm* sq, SqTable& tblMath)
{
	SqTable delegate(sq);

	delegate.NewSlot("add", SqFunction(sq, &Add, 2));
	delegate.NewSlot("sub", SqFunction(sq, &Sub, 2));
	delegate.NewSlot("scale", SqFunction(sq, &Scale, 2));
	delegate.NewSlot("rotate", SqFunction(sq, &Rotate, 2));
	delegate.NewSlot("rotater", SqFunction(sq, &Rotater, 2));
	delegate.NewSlot("nor", SqFunction(sq, &Nor, 1));
	delegate.NewSlot("perp", SqFunction(sq, &Perp, 1));
	delegate.NewSlot("angle", SqFunction(sq, &Angle, 1));
	delegate.NewSlot("angler", SqFunction(sq, &Angler, 1));
	delegate.NewSlot("len", SqFunction(sq, &Len, 1));
	delegate.NewSlot("lenSqr", SqFunction(sq, &LenSqr, 1));
	delegate.NewSlot("dist", SqFunction(sq, &Dist, 3));
	delegate.NewSlot("distSqr", SqFunction(sq, &DistSqr, 3));
	delegate.NewSlot("cross", SqFunction(sq, &Cross, 3));
	delegate.NewSlot("dot", SqFunction(sq, &Dot, 3));
	delegate.NewSlot("copy", SqFunction(sq, &Copy, 1));

	sq->GetRegistryTable().NewSlot(VEC2_DELEGATE_NAME, delegate);

	// Bind constructor function
	tblMath.NewSlot("Vec2", &Constructor);
}

