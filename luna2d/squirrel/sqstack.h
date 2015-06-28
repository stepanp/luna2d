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

#include "sqvm.h"

namespace luna2d{

template<typename T>
struct SqStack
{
	inline static void Push(HSQUIRRELVM vm, const T& value) = delete; // Push value to squirrel stack
	inline static T Get(HSQUIRRELVM vm, int index = -1) = delete; // Get value from squirrel stack
};

// By default, for const references use same implementation
// as for non-reference type
template<typename T>
struct SqStack<const T&> : public SqStack<T> {};

template<>
struct SqStack<SQInteger>
{
	inline static void Push(HSQUIRRELVM vm, SQInteger value)
	{
		sq_pushinteger(vm, value);
	}

	inline static SQInteger Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) == OT_INTEGER)
		{
			SQInteger ret;
			sq_getinteger(vm, index, &ret);
			return ret;
		}

		else if(sq_gettype(vm, index) == OT_FLOAT)
		{
			SQFloat ret;
			sq_getfloat(vm, index, &ret);
			return static_cast<SQInteger>(ret);
		}

		return 0;
	}
};

template<>
struct SqStack<SQFloat>
{
	inline static void Push(HSQUIRRELVM vm, SQFloat value)
	{
		sq_pushfloat(vm, value);
	}

	inline static SQFloat Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) == OT_FLOAT)
		{
			SQFloat ret;
			sq_getfloat(vm, index, &ret);
			return ret;
		}

		else if(sq_gettype(vm, index) == OT_INTEGER)
		{
			SQInteger ret;
			sq_getinteger(vm, index, &ret);
			return static_cast<SQFloat>(ret);
		}

		return 0;
	}
};

template<>
struct SqStack<bool>
{
	inline static void Push(HSQUIRRELVM vm, bool value)
	{
		sq_pushbool(vm, value ? SQTrue : SQFalse);
	}

	inline static bool Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_BOOL) return false;

		SQBool ret;
		sq_getbool(vm, index, &ret);
		return ret == SQTrue;
	}
};

template<>
struct SqStack<std::string>
{
	inline static void Push(HSQUIRRELVM vm, const std::string& value)
	{
		sq_pushstring(vm, value.c_str(), value.size());
	}

	inline static std::string Get(HSQUIRRELVM vm, int index = -1)
	{
		if(sq_gettype(vm, index) != OT_STRING) return "";

		const SQChar* ret;
		sq_getstring(vm, index, &ret);
		return std::string(ret);
	}
};

template<>
struct SqStack<const char*>
{
	inline static void Push(HSQUIRRELVM vm, const char* value)
	{
		sq_pushstring(vm, value, -1);
	}
};

template<size_t Size>
struct SqStack<char[Size]>
{
	inline static void Push(HSQUIRRELVM vm, const char(&value)[Size])
	{
		sq_pushstring(vm, value, Size);
	}
};

}
