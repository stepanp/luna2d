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

#include "sqobject.h"

using namespace luna2d;

SqObject::SqObject() :
	ref(std::make_shared<SqRef>(nullptr))
{
}

SqObject::SqObject(HSQUIRRELVM vm) :
	ref(std::make_shared<SqRef>(vm))
{
}

SqObject::SqObject(const std::shared_ptr<SqRef>& ref) :
	ref(ref)
{
}

std::shared_ptr<SqRef> SqObject::GetRef() const
{
	return ref;
}

SQObjectType SqObject::GetType() const
{
	if(IsNull()) return OT_NULL;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	SQObjectType ret = sq_gettype(vm, -1);
	sq_pop(vm, 1); // Pop object from stack

	return ret;
}

std::string SqObject::ToString() const
{
	if(IsNull()) return "null";

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_tostring(vm, -1);
	std::string ret = SqStack<std::string>::Get(vm, -1);
	sq_pop(vm, 2); // Pop string and object from stack

	return ret;
}

bool SqObject::IsNull() const
{
	return ref->IsNull();
}

SqObject::operator bool() const
{
	return !IsNull();
}
