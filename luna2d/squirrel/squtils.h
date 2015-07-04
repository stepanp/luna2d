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

#include "sqvm.h"
#include "lunamacro.h"

namespace luna2d{

// Pop n values from squirrel stack after goes out scope
// e.g. after return statement
class SqScopedPop
{
public:
	SqScopedPop(HSQUIRRELVM vm, int n);
	~SqScopedPop();

private:
	HSQUIRRELVM vm;
	int n;
};


// Remove value from squirrel stack at "index" after goes out scope
// e.g. after return statement
class SqScopedRemove
{
public:
	SqScopedRemove(HSQUIRRELVM vm, int index);
	~SqScopedRemove();

private:
	HSQUIRRELVM vm;
	int index;
};


// Get string represenation of squirrel type
const char* SqTypeName(SQObjectType type);


// Debug macro for print squirrel stack
#define LUNA_SQ_PRINT_STACK(vm) \
{ \
	LUNA_LOGE("%s:%d:", LUNA_FILE, __LINE__); \
	int count = sq_gettop(vm); \
	for(int i = 1; i <= count; i++) \
	{ \
		SQObjectType type = sq_gettype(vm, i); \
		\
		sq_tostring(vm, i); \
		const SQChar* value; \
		sq_getstring(vm, -1, &value); \
		\
		LUNA_LOG("%d: %s(%s)", i - count - 1, SqTypeName(type), value); \
		\
		sq_pop(vm, 1); \
	} \
}

// Debug macros for checking squirrel stack state
// before/afrer using functions working with stack
// For example:
//   LUNA_SQ_CHECK_STACK_BEGIN(vm);
//   SqTable tbl = sq->GetRootTable(vm);
//   LUNA_SQ_CHECK_STACK(vm);
//   tbl.SetSlot("slotName", "sloValue");
//   LUNA_SQ_CHECK_STACK(vm);
#define LUNA_SQ_CHECK_STACK_BEGIN(vm) \
	int _stackSize = sq_gettop(vm); \
	int _nStackSize = sq_gettop(vm); \

#define LUNA_SQ_CHECK_STACK(vm) \
	_nStackSize = sq_gettop(vm); \
	if(_stackSize != _nStackSize) \
	{ \
		LUNA_LOGE("Stack size is diffirent! %d -> %d", _stackSize, _nStackSize); \
		LUNA_SQ_PRINT_STACK(vm); \
	} \
	_stackSize = _nStackSize
}
