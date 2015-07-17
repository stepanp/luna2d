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

#include "sqptr.h"
#include "lunaindexlist.h"

namespace luna2d{

template<typename Ret, typename ... Args>
class SqFunctionProxy
{
	typedef SqFunctionProxy<Ret, Args...> Proxy;
	typedef std::function<Ret(Args...)> Func;

public:
	SqFunctionProxy(const Func& func) : func(func) {}

private:
	Func func;

public:
	static SQInteger Callback(HSQUIRRELVM vm)
	{
		if(sq_gettype(vm, -1) != OT_USERDATA) return SQ_ERROR;

		SQUserPointer proxyPtr;
		sq_getuserdata(vm, -1, &proxyPtr, nullptr);

		Proxy* proxy = *static_cast<Proxy**>(proxyPtr);
		SqStack<Ret>::Push(vm, Call(vm, proxy, LUNAMakeIndexList<Args...>()));

		return 1;
	}

	template<size_t ... Index>
	static Ret Call(HSQUIRRELVM vm, Proxy* proxy, LUNAIndexList<Index...>)
	{
		return proxy->func(SqStack<Args>::Get(vm, Index + 2)...);
	}
};


template<typename ... Args>
class SqFunctionProxy<void, Args...>
{
	typedef SqFunctionProxy<void, Args...> Proxy;
	typedef std::function<void(Args...)> Func;

public:
	SqFunctionProxy(const Func& func) : func(func) {}

private:
	Func func;

public:
	static SQInteger Callback(HSQUIRRELVM vm)
	{
		if(sq_gettype(vm, -1) != OT_USERDATA) return SQ_ERROR;

		SQUserPointer proxyPtr;
		sq_getuserdata(vm, -1, &proxyPtr, nullptr);

		Proxy* proxy = *static_cast<Proxy**>(proxyPtr);
		Call(vm, proxy, LUNAMakeIndexList<Args...>());

		return 0;
	}

	template<size_t ... Index>
	static void Call(HSQUIRRELVM vm, Proxy* proxy, LUNAIndexList<Index...>)
	{
		proxy->func(SqStack<Args>::Get(vm, Index + 2)...);
	}
};


template<typename Ret, typename Class, typename ... Args>
class SqMethodProxy
{
	typedef SqMethodProxy<Ret, Class, Args...> Proxy;
	typedef Ret (Class::*Method)(Args ...);

public:
	SqMethodProxy(Class* obj, Method method) : obj(obj), method(method) {}

private:
	Class* obj;
	Method method;

public:
	static SQInteger Callback(HSQUIRRELVM vm)
	{
		if(sq_gettype(vm, -1) != OT_USERDATA) return SQ_ERROR;

		SQUserPointer proxyPtr;
		sq_getuserdata(vm, -1, &proxyPtr, nullptr);

		Proxy* proxy = *static_cast<Proxy**>(proxyPtr);
		SqStack<Ret>::Push(vm, Call(vm, proxy, LUNAMakeIndexList<Args...>()));

		return 1;
	}

	template<size_t ... Index>
	static Ret Call(HSQUIRRELVM vm, Proxy* proxy, LUNAIndexList<Index...>)
	{
		return (proxy->obj->*proxy->method)(SqStack<Args>::Get(vm, Index + 2)...);
	}
};


template<typename Class, typename ... Args>
class SqMethodProxy<void, Class, Args ...>
{
	typedef SqMethodProxy<void, Class, Args...> Proxy;
	typedef void (Class::*Method)(Args ...);

public:
	SqMethodProxy(Class* obj, Method method) : obj(obj), method(method) {}

private:
	Class* obj;
	Method method;

public:
	static SQInteger Callback(HSQUIRRELVM vm)
	{
		if(sq_gettype(vm, -1) != OT_USERDATA) return SQ_ERROR;

		SQUserPointer proxyPtr;
		sq_getuserdata(vm, -1, &proxyPtr, nullptr);

		Proxy* proxy = *static_cast<Proxy**>(proxyPtr);
		Call(vm, proxy, LUNAMakeIndexList<Args...>());

		return 1;
	}

	template<size_t ... Index>
	static void Call(HSQUIRRELVM vm, Proxy* proxy, LUNAIndexList<Index...>)
	{
		(proxy->obj->*proxy->method)(SqStack<Args>::Get(vm, Index + 2)...);
	}
};


template<typename Class, typename ... Args>
class SqConstructProxy
{
public:
	static SQInteger Callback(HSQUIRRELVM vm)
	{
		SqSetInstance(vm, 1, new SqPtr<Class>(CreateInstance(vm, LUNAMakeIndexList<Args...>())));
		return 1;
	}

	template<size_t ... Index>
	static std::shared_ptr<Class> CreateInstance(HSQUIRRELVM vm, LUNAIndexList<Index...>)
	{
		return std::make_shared<Class>(SqStack<Args>::Get(vm, Index + 2)...);
	}
};

}
