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
#include "sqfunction.h"
#include "squtils.h"

namespace luna2d{

class SqTable : public SqObject
{
	friend struct SqStack<SqTable>;

public:
	SqTable();
	SqTable(SqVm* vm);
	SqTable(HSQUIRRELVM vm);
	SqTable(const SqTable& fn);

private:
	SqTable(const std::shared_ptr<SqRef>& ref);

public:
	template<typename T>
	T GetSlot(const std::string& name) const
	{
		if(IsNull()) return T();

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		if(SQ_FAILED(sq_get(vm, -2))) sq_pushnull(vm);

		SqScopedPop pop(vm, 2); // Pop table and slot value from stack
		return SqStack<T>::Get(vm, -1);
	}

	template<typename T>
	bool NewSlot(const std::string& name, const T& t)
	{
		if(IsNull()) return false;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		SqStack<T>::Push(vm, t);

		if(SQ_FAILED(sq_newslot(vm, -3, false)))
		{
			sq_pop(vm, 2); // Pop table and name from stack
			return false;
		}

		sq_pop(vm, 1); // Pop table from stack
		return true;
	}

	template<typename T>
	bool SetSlot(const std::string& name, const T& t)
	{
		if(IsNull()) return false;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		SqStack<T>::Push(vm, t);

		if(SQ_FAILED(sq_set(vm, -3)))
		{
			sq_pop(vm, 2); // Pop table and name from stack
			return false;
		}

		sq_pop(vm, 1); // Pop table from stack
		return true;
	}

	SqTable& operator=(const SqTable& fn);

	// Inline getters for most common used types
	inline SqTable GetTable(const std::string& name) const { return GetSlot<SqTable>(name); }
	inline SqFunction GetFunction(const std::string& name) const { return GetSlot<SqFunction>(name); }
	inline bool GetBool(const std::string& name) const { return GetSlot<bool>(name); }
	inline int GetInt(const std::string& name) const { return GetSlot<int>(name); }
	inline float GetFloat(const std::string& name) const { return GetSlot<float>(name); }
	inline std::string GetString(const std::string& name) const { return GetSlot<std::string>(name); }
};


template<>
struct SqStack<SqTable>
{
	inline static void Push(HSQUIRRELVM vm, const SqTable& tbl)
	{
		SqStack<SqObject>::Push(vm, tbl);
	}

	inline static SqTable Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_TABLE) return SqTable();
		return SqTable(SqStack<std::shared_ptr<SqRef>>::Get(vm, index));
	}
};

}