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

#include "sqobject.h"
#include "squtils.h"

namespace luna2d{

class SqTable;
class SqFunction;

class SqArray : public SqObject
{
	friend struct SqStack<SqArray>;

public:
	SqArray();
	SqArray(SqVm* vm, int count = 0);
	SqArray(HSQUIRRELVM vm, int count = 0);
	SqArray(const SqArray& array);

private:
	SqArray(const std::shared_ptr<SqRef>& ref);

public:
	int GetCount() const;
	void Clear();
	void Resize(int size);
	void Reverse();

	template<typename T>
	void AppendValue(const T& t)
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<T>::Push(vm, t);
		sq_arrayappend(vm, -2);
		sq_pop(vm, 1); // Pop array from stack
	}

	template<typename T>
	void InsertValue(int index, const T& t)
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<T>::Push(vm, t);
		sq_arrayinsert(vm, -2, index);
		sq_pop(vm, 1); // Pop array from stack
	}

	template<typename T>
	T GetValue(int index) const
	{
		if(IsNull()) return T();

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<int>::Push(vm, index);
		if(SQ_FAILED(sq_get(vm, -2))) sq_pushnull(vm);

		SqScopedPop pop(vm, 2); // Pop array and value from stack
		return SqStack<T>::Get(vm, -1);
	}

	template<typename T>
	void SetValue(int index, const T& t)
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<int>::Push(vm, index);
		SqStack<T>::Push(vm, t);

		if(SQ_FAILED(sq_set(vm, -3)))
		{
			sq_pop(vm, 3); // Pop array, index and value from stack
			return;
		}

		sq_pop(vm, 1); // Pop array from stack
	}

	void RemoveValue(int index);

	SqArray& operator=(const SqArray& array);

	// Inline getters for most common used types
	inline bool GetBool(int index) const { return GetValue<bool>(index); }
	inline int GetInt(int index) const { return GetValue<int>(index); }
	inline float GetFloat(int index) const { return GetValue<float>(index); }
	inline std::string GetString(int index) const { return GetValue<std::string>(index); }
	inline SqArray GetArray(int index) const { return GetValue<SqArray>(index); }
	SqTable GetTable(int index) const;
	SqFunction GetFunction(int index) const;
};


template<>
struct SqStack<SqArray> : public SqStack<SqObject>
{
	inline static SqArray Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_ARRAY) return SqArray();
		return SqArray(SqStack<std::shared_ptr<SqRef>>::Get(vm, index));
	}
};

}
