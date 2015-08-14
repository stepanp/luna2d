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

#include "lunasquirrel.h"
#include "lunaglm.h"

namespace luna2d{

const std::string VEC2_DELEGATE_NAME = "glm::vec2";

void BindVec2(SqVm* sq, SqTable& tblMath);

template<>
struct SqStack<glm::vec2>
{
	inline static void Push(HSQUIRRELVM vm, const glm::vec2& vec)
	{
		// Create vec2 table
		sq_newtableex(vm, 2);
		sq_pushstring(vm, "x", 1);
		sq_pushfloat(vm, vec.x);
		sq_rawset(vm, -3);
		sq_pushstring(vm, "y", 1);
		sq_pushfloat(vm, vec.y);
		sq_rawset(vm, -3);

		// Set delegate to vec2 table
		sq_pushregistrytable(vm);
		sq_pushstring(vm, VEC2_DELEGATE_NAME.c_str(), VEC2_DELEGATE_NAME.length());
		sq_rawget(vm, -2);
		sq_remove(vm, -2);
		sq_setdelegate(vm, -2);
	}

	// Set new x and y values for table at index of stack
	// To avoid creation a copy of vector table
	inline static void Set(HSQUIRRELVM vm, const glm::vec2& vec, int index)
	{
		sq_push(vm, index);
		sq_pushstring(vm, "x", 1);
		sq_pushfloat(vm, vec.x);
		sq_rawset(vm, -3);
		sq_pushstring(vm, "y", 1);
		sq_pushfloat(vm, vec.y);
		sq_rawset(vm, -3);
		sq_pop(vm, 1);
	}

	inline static glm::vec2 Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_TABLE) return glm::vec2();

		glm::vec2 ret;

		sq_pushstring(vm, "x", 1);
		if(SQ_SUCCEEDED(sq_rawget(vm, index)))
		{
			sq_getfloat(vm, -1, &ret.x);
			sq_pop(vm, 1);
		}

		sq_pushstring(vm, "y", 1);
		if(SQ_SUCCEEDED(sq_rawget(vm, index)))
		{
			sq_getfloat(vm, -1, &ret.y);
			sq_pop(vm, 1);
		}

		return ret;
	}
};

}
