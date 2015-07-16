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

namespace luna2d{

class SqTypeTags
{
	friend class SqVm;

protected:
	static size_t counter;
	static std::vector<void(*)()> cleaners;

	static void Reset();
};


template<typename Class>
class SqClassInfo : public SqTypeTags
{
private:
	static size_t typeTag;

public:
	static size_t GetTypeTag()
	{
		return typeTag;
	}

	static void ResetTypeTag()
	{
		typeTag = -1;
		SqTypeTags::counter--;
	}

	static bool IsRegistered(SqVm* vm)
	{
		if(typeTag == -1) return false;

		sq_pushregistrytable(*vm);
		SqStack<std::string>::Push(*vm, SQ_CLASSES_REGISTRY);
		sq_get(*vm, -2);

		sq_pushinteger(*vm, static_cast<SQInteger>(typeTag));
		if(SQ_FAILED(sq_get(*vm, -2)))
		{
			sq_pop(*vm, 2); // Pop registry table and classes registry from stack
			return false;
		}

		sq_pop(*vm, 3); // Pop registry table, classes registry amd class value from stack
		return true;
	}

	static void Register(SqVm* vm, const SqObject& cls)
	{
		if(typeTag != -1 || cls.GetType() != OT_CLASS) return;

		typeTag = SqTypeTags::counter++;
		SqTypeTags::cleaners.push_back(&ResetTypeTag);

		sq_pushregistrytable(*vm);
		SqStack<std::string>::Push(*vm, SQ_CLASSES_REGISTRY);
		sq_get(*vm, -2);

		sq_pushinteger(*vm, static_cast<SQInteger>(typeTag));
		SqStack<SqObject>::Push(*vm, cls);
		sq_arrayappend(*vm, -3);

		sq_pop(*vm, 3); // Pop registry table, classes registry and typeTag from stack
	}
};

template<typename Class>
size_t SqClassInfo<Class>::typeTag = -1;


template<typename Class>
struct SqStack<SqClassInfo<Class>>
{
	static void Push(HSQUIRRELVM vm)
	{
		size_t typeTag = SqClassInfo<Class>::GetTypeTag();

		if(typeTag == -1)
		{
			sq_pushnull(vm);
			return;
		}

		sq_pushregistrytable(vm);
		SqStack<std::string>::Push(vm, SQ_CLASSES_REGISTRY);
		sq_get(vm, -2);
		sq_remove(vm, -2); // Remove registry table from stack

		sq_pushinteger(vm, static_cast<SQInteger>(typeTag));
		if(SQ_FAILED(sq_get(vm, -2)))
		{
			sq_pop(vm, 2); // Pop classes registry and typeTag from stack
			sq_pushnull(vm);
			return;
		}

		sq_remove(vm, -2); // Remove classes registry from stack
	}
};

}
