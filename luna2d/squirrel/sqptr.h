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

#include "sqclassinfo.h"

namespace luna2d{

template<typename T>
class SqPtr
{
	union SqPtrType
	{
		std::shared_ptr<T> shared;
		std::weak_ptr<T> weak;
	};

public:
	SqPtr(const std::shared_ptr<T>& ptr) : isWeak(false), type(ptr) {}
	SqPtr(const std::weak_ptr<T>& ptr) : isWeak(true), type(ptr) {}

private:
	bool isWeak;
	SqPtrType type;

public:
	std::shared_ptr<T> ToShared()
	{
		if(isWeak) return nullptr; // Don't allow implicit casting from weak_ptr to shared_ptr
		else return type.shared;
	}

	std::weak_ptr<T> ToWeak()
	{
		if(isWeak) return type.weak;
		else return type.shared;
	}
};


template<typename T>
struct SqStack<SqPtr<T>>
{
	template<typename PtrType>
	inline static void Push(HSQUIRRELVM vm, const PtrType& value)
	{
		SqStack<SqClassInfo<T>>::Push(vm);
		if(sq_gettype(vm, -1) == OT_NULL) return;

		sq_createinstance(vm, -1);
		sq_setinstanceup(vm, -1, static_cast<SQUserPointer>(new SqPtr<T>(value)));
		sq_setreleasehook(vm, -1, [](SQUserPointer ptr, SQInteger) -> SQInteger
		{
			SqPtr<T>* wrapPtr = static_cast<SqPtr<T>*>(ptr);
			delete wrapPtr;
			return 0;
		});
	}

	inline static SqPtr<T>* Get(HSQUIRRELVM vm, int index = -1)
	{
		return nullptr;
	}
};


template<typename T>
struct SqStack<std::shared_ptr<T>>
{
	inline static void Push(HSQUIRRELVM vm, const std::shared_ptr<T>& value)
	{
		if(!value)
		{
			sq_pushnull(vm);
			return;
		}

		SqStack<SqPtr<T>>::Push(vm, value);
	}

	inline static std::shared_ptr<T> Get(HSQUIRRELVM vm, int index = -1)
	{
		return SqStack<SqPtr<T>>::Get(vm, index)->ToShared();
	}
};


template<typename T>
struct SqStack<std::weak_ptr<T>>
{
	inline static void Push(HSQUIRRELVM vm, const std::weak_ptr<T>& value)
	{
		if(value.expired())
		{
			sq_pushnull(vm);
			return;
		}

		SqStack<SqPtr<T>>::Push(vm, value);
	}

	inline static std::weak_ptr<T> Get(HSQUIRRELVM vm, int index = -1)
	{
		return SqStack<SqPtr<T>>::Get(vm, index)->ToWeak();
	}
};

}
