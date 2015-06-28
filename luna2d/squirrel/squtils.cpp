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

#include "squtils.h"

using namespace luna2d;

SqScopedPop::SqScopedPop(HSQUIRRELVM vm, int n) :
	vm(vm), n(n)
{
}

SqScopedPop::~SqScopedPop()
{
	sq_pop(vm, n);
}


SqScopedRemove::SqScopedRemove(HSQUIRRELVM vm, int index) :
	vm(vm), index(index)
{
}

SqScopedRemove::~SqScopedRemove()
{
	sq_remove(vm, index);
}


// Get string represenation of squirrel type
const char* luna2d::SqTypeName(SQObjectType type)
{
	switch(type)
	{
	case OT_NULL:
		return "null";
	case OT_BOOL:
		return "bool";
	case OT_INTEGER:
		return "integer";
	case OT_FLOAT:
		return "float";
	case OT_STRING:
		return "string";
	case OT_TABLE:
		return "table";
	case OT_ARRAY:
		return "array";
	case OT_USERDATA:
		return "userdata";
	case OT_CLOSURE:
		return "function";
	case OT_NATIVECLOSURE:
		return "C function";
	case OT_GENERATOR:
		return "generator";
	case OT_USERPOINTER:
		return "userpointer";
	case OT_CLASS:
		return "class";
	case OT_INSTANCE:
		return "instance";
	case OT_WEAKREF:
		return "weak reference";
	default:
		return "unknown";
	}
}
