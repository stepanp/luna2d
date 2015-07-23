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

#include "sqarray.h"
#include "sqtable.h"
#include "sqfunction.h"

using namespace luna2d;

SqArray::SqArray() : SqObject() {}
SqArray::SqArray(SqVm* vm, int count) : SqArray(*vm, count) {}
SqArray::SqArray(HSQUIRRELVM vm, int count) : SqObject(vm)
{
	if(vm)
	{
		sq_newarray(vm, count);
		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1);
	}
}

SqArray::SqArray(const SqArray& array) : SqObject(array.GetRef()) {}
SqArray::SqArray(const std::shared_ptr<SqRef>& ref) : SqObject(ref) {}

int SqArray::GetCount() const
{
	if(IsNull()) return 0;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	int ret = sq_getsize(vm, -1);
	sq_pop(vm, 1); // Pop array from stack

	return ret;
}

void SqArray::Clear()
{
	if(IsNull()) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_clear(vm, -1);
	sq_pop(vm, 1);
}

void SqArray::Resize(int size)
{
	if(IsNull()) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_arrayresize(vm, -1, size);
	sq_pop(vm, 1); // Pop array from stack
}

void SqArray::Reverse()
{
	if(IsNull()) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_arrayreverse(vm, -1);
	sq_pop(vm, 1); // Pop array from stack
}

void SqArray::RemoveValue(int index)
{
	if(IsNull()) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_arrayremove(vm, -1, index);
	sq_pop(vm, 1); // Pop array from stack
}

SqArray& SqArray::operator=(const SqArray& array)
{
	ref = array.GetRef();
	return *this;
}

SqTable SqArray::GetTable(int index) const
{
	return GetValue<SqTable>(index);
}

SqFunction SqArray::GetFunction(int index) const
{
	return GetValue<SqFunction>(index);
}
