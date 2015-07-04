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

#include "sqany.h"
#include "sqarray.h"
#include "sqtable.h"
#include "sqfunction.h"

using namespace luna2d;

SqAny::SqAny() : SqObject() {}
SqAny::SqAny(SqVm* vm) : SqObject(*vm) {}
SqAny::SqAny(HSQUIRRELVM vm) : SqObject(vm) {}
SqAny::SqAny(const SqAny& any) : SqObject(any.GetRef()) {}
SqAny::SqAny(const std::shared_ptr<SqRef>& ref) : SqObject(ref) {}

SQObjectType SqAny::GetType() const
{
	if(IsNull()) return OT_NULL;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	SQObjectType ret = sq_gettype(vm, -1);
	sq_pop(vm, 1); // Pop object from stack

	return ret;
}

SqAny& SqAny::operator=(const SqAny& fn)
{
	ref = fn.GetRef();
	return *this;
}

SqArray SqAny::ToArray() const
{
	return To<SqArray>();
}

SqTable SqAny::ToTable() const
{
	return To<SqTable>();
}

SqFunction SqAny::ToFunction() const
{
	return To<SqFunction>();
}
