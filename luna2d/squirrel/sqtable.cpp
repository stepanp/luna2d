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

#include "sqtable.h"
#include "sqarray.h"

using namespace luna2d;

SqTable::SqTable() : SqObject() {}
SqTable::SqTable(SqVm* vm) : SqTable(*vm) {}
SqTable::SqTable(HSQUIRRELVM vm) : SqObject(vm)
{
	if(vm)
	{
		sq_newtable(vm);
		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1);
	}
}

SqTable::SqTable(const SqTable& tbl) : SqObject(tbl.GetRef()) {}
SqTable::SqTable(const std::shared_ptr<SqRef>& ref) : SqObject(ref) {}

int SqTable::GetCount() const
{
	if(IsNull()) return 0;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	int ret = sq_getsize(vm, -1);
	sq_pop(vm, 1); // Pop table from stack

	return ret;
}

void SqTable::Clear()
{
	if(IsNull() || GetType() != OT_TABLE) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	sq_clear(vm, -1);
	sq_pop(vm, 1);
}

bool SqTable::HasSlot(const std::string& name) const
{
	if(IsNull()) return false;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	SqStack<std::string>::Push(vm, name);
	if(SQ_FAILED(sq_get(vm, -2)))
	{
		sq_pop(vm, 1); // Pop table from stack
		return false;
	}

	sq_pop(vm, 2); // Pop table and slot value from stack
	return true;
}

void SqTable::RemoveSlot(const std::string& name)
{
	if(IsNull() || GetType() == OT_CLASS) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqObject>::Push(vm, *this);
	SqStack<std::string>::Push(vm, name);
	sq_deleteslot(vm, -2, false);
	sq_pop(vm, 1); // Pop table from stack
}

SqTable SqTable::GetDelegate() const
{
	if(IsNull() || GetType() != OT_TABLE) return SqTable();

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqTable>::Push(vm, *this);
	sq_getdelegate(vm, -1);

	SqScopedPop(vm, 2); // Pop table and delegate from stack
	return SqStack<SqTable>::Get(vm, -1);
}

void SqTable::SetDelegate(const SqTable& delegate)
{
	if(IsNull() || GetType() != OT_TABLE) return;

	HSQUIRRELVM vm = ref->GetVm();

	SqStack<SqTable>::Push(vm, *this);
	SqStack<SqTable>::Push(vm, delegate);
	sq_setdelegate(vm, -2);

	sq_pop(vm, 1); // Pop table from stack
}

SqTable& SqTable::operator=(const SqTable& tbl)
{
	ref = tbl.GetRef();
	return *this;
}

SqArray SqTable::GetArray(const std::string& name) const
{
	return GetSlot<SqArray>(name);
}
