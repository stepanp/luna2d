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
public:
	SqPtr(const std::shared_ptr<T>& ptr) : isWeak(false), shared(ptr) {}
	SqPtr(const std::weak_ptr<T>& ptr) : isWeak(true), weak(ptr) {}
	~SqPtr() { isWeak ? weak.~weak_ptr() : shared.~shared_ptr(); }

private:
	bool isWeak;
	union
	{
		std::shared_ptr<T> shared;
		std::weak_ptr<T> weak;
	};

public:
	std::shared_ptr<T> ToShared()
	{
		if(isWeak) return nullptr; // Don't allow implicit casting from weak_ptr to shared_ptr
		else return shared;
	}

	std::weak_ptr<T> ToWeak()
	{
		if(isWeak) return weak;
		else return shared;
	}
};


template<typename T>
void SqSetInstance(HSQUIRRELVM vm, int index, SqPtr<T>* instance)
{
	sq_setinstanceup(vm, index, instance);
	sq_setreleasehook(vm, index, [](SQUserPointer ptr, SQInteger) -> SQInteger
	{
		SqPtr<T>* wrapPtr = static_cast<SqPtr<T>*>(ptr);
		delete wrapPtr;
		return 0;
	});
}


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

		SqStack<SqClassInfo<T>>::Push(vm);
		if(sq_gettype(vm, -1) == OT_NULL) return;

		sq_createinstance(vm, -1);
		sq_remove(vm, -2); // Remove class from stack

		SqSetInstance(vm, -1, new SqPtr<T>(value));
	}

	inline static std::shared_ptr<T> Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_INSTANCE) return nullptr;

		size_t typeTag = SqClassInfo<T>::GetTypeTag();
		if(typeTag == -1) return nullptr;

		SQUserPointer ptr = nullptr;
		if(SQ_FAILED(sq_getinstanceup(vm, index, &ptr, reinterpret_cast<SQUserPointer>(typeTag)))) return nullptr;

		return static_cast<SqPtr<T>*>(ptr)->ToShared();
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

		SqStack<SqClassInfo<T>>::Push(vm);
		if(sq_gettype(vm, -1) == OT_NULL) return;

		sq_createinstance(vm, -1);
		sq_remove(vm, -2); // Remove class from stack

		SqSetInstance(vm, -1, new SqPtr<T>(value));
	}

	inline static std::weak_ptr<T> Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_INSTANCE) return  std::weak_ptr<T>();

		size_t typeTag = SqClassInfo<T>::GetTypeTag();
		if(typeTag == -1) return  std::weak_ptr<T>();

		SQUserPointer ptr = nullptr;
		if(SQ_FAILED(sq_getinstanceup(vm, index, &ptr, reinterpret_cast<SQUserPointer>(typeTag)))) return std::weak_ptr<T>();

		return static_cast<SqPtr<T>*>(ptr)->ToWeak();
	}
};

}
