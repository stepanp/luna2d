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

namespace luna2d{

class SqArray;

class SqTable : public SqObject
{
	friend struct SqStack<SqTable>;

public:
	SqTable();
	SqTable(SqVm* vm);
	SqTable(HSQUIRRELVM vm);
	SqTable(const SqTable& tbl);

private:
	SqTable(const std::shared_ptr<SqRef>& ref);

public:
	int GetCount() const;
	void Clear();
	bool HasSlot(const std::string& name) const;

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
	void NewSlot(const std::string& name, const T& t, bool isStatic = false)
	{
		if(IsNull() || GetType() == OT_INSTANCE) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		SqStack<T>::Push(vm, t);

		if(SQ_FAILED(sq_newslot(vm, -3, isStatic)))
		{
			sq_pop(vm, 3); // Pop table, name and value from stack
			return;
		}

		sq_pop(vm, 1); // Pop table from stack
	}

	template<typename T>
	void SetSlot(const std::string& name, const T& t)
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<std::string>::Push(vm, name);
		SqStack<T>::Push(vm, t);

		if(SQ_FAILED(sq_set(vm, -3)))
		{
			sq_pop(vm, 3); // Pop table, name and value from stack
			return;
		}

		sq_pop(vm, 1); // Pop table from stack
	}

	void RemoveSlot(const std::string& name);

	template<typename Ret, typename ... Args>
	Ret CallMethod(const std::string& name, const Args& ... args) const
	{
		SqFunction fn = GetFunction(name);
		if(fn.IsNull()) return Ret();

		return fn.CallWithEnv<Ret>(*this, args...);
	}

	template<typename ... Args>
	void CallMethod(const std::string& name, const Args& ... args) const
	{
		SqFunction fn = GetFunction(name);
		if(fn.IsNull()) return;

		fn.CallWithEnv<void>(*this, args...);
	}

	SqTable GetDelegate() const;
	void SetDelegate(const SqTable& delegate);
	inline void RemoveDelegate() { SetDelegate(SqTable()); }

	SqTable& operator=(const SqTable& tbl);

	// Getters for most common used types
	inline bool GetBool(const std::string& name) const { return GetSlot<bool>(name); }
	inline int GetInt(const std::string& name) const { return GetSlot<int>(name); }
	inline float GetFloat(const std::string& name) const { return GetSlot<float>(name); }
	inline std::string GetString(const std::string& name) const { return GetSlot<std::string>(name); }
	inline SqTable GetTable(const std::string& name) const { return GetSlot<SqTable>(name); }
	SqArray GetArray(const std::string& name) const;
	SqFunction GetFunction(const std::string& name) const { return GetSlot<SqFunction>(name); }
};


template<>
struct SqStack<SqTable> : public SqStack<SqObject>
{
	inline static SqTable Get(HSQUIRRELVM vm, int index = -1)
	{
		SQObjectType type = sq_gettype(vm, index);
		if(type != OT_TABLE && type != OT_CLASS && type != OT_INSTANCE) return SqTable();
		return SqTable(SqStack<std::shared_ptr<SqRef>>::Get(vm, index));
	}
};

}
