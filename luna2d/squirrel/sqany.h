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

namespace luna2d{

class SqAny : public SqObject
{
	friend struct SqStack<SqAny>;

public:
	SqAny();
	SqAny(SqVm* vm);
	SqAny(HSQUIRRELVM vm);
	SqAny(const SqAny& any);

	template<typename T>
	SqAny(SqVm* vm, const T& t) : SqObject(*vm)
	{
		FromValue(t);
	}

private:
	SqAny(const std::shared_ptr<SqRef>& ref);

public:
	SQObjectType GetType() const;

	template<typename T>
	T To() const
	{
		if(IsNull()) return T();

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqScopedPop pop(vm, 1); // Pop object from stack
		return SqStack<T>::Get(vm, -1);
	}

	template<typename T>
	void FromValue(const T& t)
	{
		HSQUIRRELVM vm = ref->GetVm();

		SqStack<T>::Push(vm, t);
		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1); // Pop object from stack
	}

	SqAny& operator=(const SqAny& fn);

	// Inline getters for most common used types
	inline bool ToBool() const { return To<bool>(); }
	inline int ToInt() const { return To<int>(); }
	inline float ToFloat() const { return To<float>(); }
	inline std::string ToString() const { return To<std::string>(); }
	inline SqTable ToTable() const { return To<SqTable>(); }
	inline SqFunction ToFunction() const { return To<SqFunction>(); }
};


template<>
struct SqStack<SqAny>
{
	inline static void Push(HSQUIRRELVM vm, const SqAny& fn)
	{
		SqStack<std::shared_ptr<SqRef>>::Push(vm, fn.GetRef());
	}

	inline static SqAny Get(HSQUIRRELVM vm, int index = -1)
	{
		return SqAny(SqStack<std::shared_ptr<SqRef>>::Get(vm, index));
	}
};

}
