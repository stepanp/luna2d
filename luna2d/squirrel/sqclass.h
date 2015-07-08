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

#include "sqtable.h"
#include "sqptr.h"

namespace luna2d{

struct _SqTypeTag
{
	_SqTypeTag(SQUserPointer base) : base(base) {}
	SQUserPointer base;
};

typedef _SqTypeTag* SqTypeTag;


template<typename Class>
class _SqClass : public SqObject
{
protected:
	_SqClass(SqVm* vm, SqTypeTag baseTypeTag) : SqObject(*vm)
	{
		if(!vm) return;

		if(!typeTag) typeTag = new _SqTypeTag(baseTypeTag);
	}

protected:
	static SqTypeTag typeTag;

public:
	static SqTypeTag GetTypeTag() { return typeTag; }
	static bool HasBase() { return typeTag && typeTag->base; }
};

template<typename Class>
SqTypeTag _SqClass<Class>::typeTag = nullptr;


struct _SqNoBase {};


template<typename Class, typename BaseClass = _SqNoBase>
class SqClass : public _SqClass<Class>
{
public:
	SqClass(SqVm* vm) : _SqClass<Class>(vm, _SqClass<BaseClass>::GetTypeTag())
	{
		if(!vm) return;
		//if(_SqClass<Class>::IsRegistered(*vm)) LUNA_RETURN_ERR("Class can be registered only once");

		sq_newclass(*vm, _SqClass<Class>::HasBase());
		sq_settypetag(*vm, -1, _SqClass<Class>::GetTypeTag());
		this->ref = std::make_shared<SqRef>(*vm, -1);
		sq_pop(*vm, 1); // Pop class from stack

		SqStack<SqTable>::Push(*vm, vm->GetClassesTable());
		sq_pushuserpointer(*vm, _SqClass<Class>::GetTypeTag());
		sq_newslot(*vm, -2, true);
		sq_pop(*vm, 1);







		/*sq_pushregistrytable(*vm);
		SqTable tblRegistry = SqStack<SqTable>::Get(*vm, -1);
		sq_pop(*vm, 1);

		SqTable tblClasses;
		if(!tblRegistry.HasSlot(SQ_CLASSES_TABLE)) tblRegistry.NewSlot(SQ_CLASSES_TABLE, SqTable(vm), true);
		else tblClasses = tblRegistry.GetTable(SQ_CLASSES_TABLE);

		/*if(_SqClass<Class>::HasBase())
		{


			SqStack<Class>::Push()
		}*
		SqStack<SqTable>::Push(*vm, tblClasses);
		sq_pushuserpointer(*vm, _SqClass<Class>::GetTypeTag());
		SqStack<SqObject>::Push(*vm, *this);
		sq_rawset(*vm, -3);*/



	}
};


template<typename Class, typename BaseClass>
struct SqStack<SqClass<Class,BaseClass>> : public SqStack<SqObject> {};

}
