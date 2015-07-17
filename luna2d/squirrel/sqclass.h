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

#include "sqfunction.h"
#include "sqptr.h"

namespace luna2d{

template<typename Class, typename BaseClass = void>
class SqClass : public SqObject
{
public:
	SqClass(SqVm* vm) : SqObject()
	{
		if(!vm) return;
		if(SqClassInfo<Class>::IsRegistered(vm)) LUNA_RETURN_ERR("Class can be registered only once");

		bool hasBase = !std::is_void<BaseClass>::value;
		if(hasBase)
		{
			if(!SqClassInfo<BaseClass>::IsRegistered(vm)) LUNA_RETURN_ERR("Baseclass must be registered before class");
			SqStack<SqClassInfo<BaseClass>>::Push(*vm);
		}

		sq_newclass(*vm, hasBase);
		ref = std::make_shared<SqRef>(*vm, -1);
		SqClassInfo<Class>::Register(vm, *this);
		sq_settypetag(*vm, -1, reinterpret_cast<SQUserPointer>(SqClassInfo<Class>::GetTypeTag()));

		sq_pop(*vm, 1); // Pop class from stack
	}

public:
	template<typename ... Args>
	void BindConstructor()
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, "constructor");
		sq_newclosure(vm, &SqConstructProxy<Class,Args...>::Callback, 0);
		sq_setparamscheck(vm, sizeof...(Args) + 1, nullptr);
		sq_newslot(vm, -3, false);

		sq_pop(vm, 1); // Pop class from stack
	}

	template<typename Ret, typename ... Args>
	void BindMethod(const std::string& name, Ret (Class::*method)(Args ...))
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqFunction fn(vm);
		fn.Bind(method);

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		SqStack<SqFunction>::Push(vm, fn);
		sq_newslot(vm, -3, false);

		sq_pop(vm, 1); // Pop class from stack
	}
};


template<typename Class, typename BaseClass>
struct SqStack<SqClass<Class,BaseClass>> : public SqStack<SqObject> {};

}
