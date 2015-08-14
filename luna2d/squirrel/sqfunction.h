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
#include "sqproxy.h"
#include "squtils.h"

namespace luna2d{

class SqFunction : public SqObject
{
	friend struct SqStack<SqFunction>;

public:
	SqFunction();
	SqFunction(SqVm* vm);
	SqFunction(HSQUIRRELVM vm);
	SqFunction(const SqFunction& fn);

	// Binding constructors
	template<typename Ret, typename ... Args>
	SqFunction(SqVm* vm, const std::function<Ret(Args...)>& func) : SqFunction(vm)
	{
		Bind<Ret,Args...>(func);
	}

	template<typename Ret, typename ... Args>
	SqFunction(SqVm* vm, Ret(*func)(Args...)) : SqFunction(vm)
	{
		Bind<Ret,Args...>(func);
	}

	template<typename Ret, typename Class, typename ... Args>
	SqFunction(SqVm* vm, Class* obj, Ret (Class::*method)(Args ...))  : SqFunction(vm)
	{
		Bind<Ret,Class,Args...>(obj, method);
	}

	SqFunction(SqVm* vm, SQFUNCTION func, int nparams = -1);

private:
	SqFunction(const std::shared_ptr<SqRef>& ref);

private:
	// Push arguments list to stack
	void PushArgs(HSQUIRRELVM) const {}

	template<typename T>
	void PushArgs(HSQUIRRELVM vm, const T& t) const
	{
		SqStack<T>::Push(vm, t);
	}

	template<typename T, typename ... Args>
	void PushArgs(HSQUIRRELVM vm, const T& t, const Args& ... args) const
	{
		SqStack<T>::Push(vm, t);
		PushArgs(vm, args...);
	}

	template<typename Proxy>
	void PushProxy(HSQUIRRELVM vm, Proxy* proxy) const
	{
		SQUserPointer proxyPtr = sq_newuserdata(vm, sizeof(Proxy*));
		memcpy(proxyPtr, &proxy, sizeof(Proxy*));

		sq_setreleasehook(vm, -1, &SqFunction::OnProxyGc<Proxy>);
	}

	template<typename Proxy>
	static SQInteger OnProxyGc(SQUserPointer ptr, SQInteger size)
	{
		Proxy* proxyPtr = *reinterpret_cast<Proxy**>(ptr);
		delete proxyPtr;

		return 1;
	}

public:
	// Bind function
	template<typename Ret, typename ... Args>
	void Bind(const std::function<Ret(Args ...)>& func)
	{
		HSQUIRRELVM vm = ref->GetVm();
		if(!vm) return;

		PushProxy(vm, new SqFunctionProxy<Ret,Args...>(func));
		sq_newclosure(vm, &SqFunctionProxy<Ret,Args...>::Callback, 1);
		sq_setparamscheck(vm, sizeof...(Args) + 1, nullptr);

		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1); // Pop function from stack
	}

	// Helper for automatic detecting argument types of binding function
	template<typename Ret, typename ... Args>
	void Bind(Ret(*ptr)(Args ...))
	{
		if(!ref->GetVm()) return;

		std::function<Ret(Args...)> func = ptr;
		Bind(func);
	}

	// Bind method as function
	template<typename Ret, typename Class, typename ... Args>
	void Bind(Class* obj, Ret (Class::*method)(Args ...))
	{
		HSQUIRRELVM vm = ref->GetVm();
		if(!vm) return;

		PushProxy(vm, new SqMethodProxy<Ret,Class,Args...>(obj, method));
		sq_newclosure(vm, &SqMethodProxy<Ret,Class,Args...>::Callback, 1);
		sq_setparamscheck(vm, sizeof...(Args) + 1, nullptr);

		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1); // Pop function from stack
	}

	// Bind method for class
	template<typename Ret, typename Class, typename ... Args>
	void Bind(Ret (Class::*method)(Args ...))
	{
		HSQUIRRELVM vm = ref->GetVm();
		if(!vm) return;

		PushProxy(vm, new SqClassProxy<Ret,Class,Args...>( method));
		sq_newclosure(vm, &SqClassProxy<Ret,Class,Args...>::Callback, 1);
		sq_setparamscheck(vm, sizeof...(Args) + 1, nullptr);

		ref = std::make_shared<SqRef>(vm, -1);
		sq_pop(vm, 1); // Pop function from stack
	}

	// Bind native squirrel closure
	void Bind(SQFUNCTION func, int nparams = -1);

	// Call function and get return value
	template<typename Ret, typename ... Args>
	Ret Call(const Args& ... args) const
	{
		if(IsNull()) return Ret();

		return CallWithEnv<Ret>(nullptr, args...);
	}

	// Call function without getting return value
	template<typename ... Args>
	void Call(const Args& ... args) const
	{
		if(IsNull()) return;

		CallWithEnv(nullptr, args...);
	}

	// Call function with given environment
	template<typename Ret, typename Env, typename ... Args>
	Ret CallWithEnv(const Env& env, const Args& ... args) const
	{
		if(IsNull()) return Ret();

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<Env>::Push(vm, env);
		PushArgs(vm, args...);

		sq_call(vm, sizeof...(args) + 1, true, true);

		SqScopedPop pop(vm, 2); // Pop function object and return value from stack
		return SqStack<Ret>::Get(vm, -1);
	}

	// Call function with given environmen without return value
	template<typename Env, typename ... Args>
	void CallWithEnv(const Env& env, const Args& ... args) const
	{
		if(IsNull()) return;

		HSQUIRRELVM vm = ref->GetVm();

		SqStack<SqObject>::Push(vm, *this);
		SqStack<Env>::Push(vm, env);
		PushArgs(vm, args...);

		sq_call(vm, sizeof...(args) + 1, false, true);
		sq_pop(vm, 1); // Pop function object from stack
	}

	SqFunction& operator=(const SqFunction& fn);
};


template<>
struct SqStack<SqFunction> : public SqStack<SqObject>
{
	inline static SqFunction Get(HSQUIRRELVM vm, int index = -1)
	{
		SQObjectType type = sq_gettype(vm, index);
		if(type != OT_CLOSURE && type != OT_NATIVECLOSURE) return SqFunction();
		return SqFunction(SqStack<std::shared_ptr<SqRef>>::Get(vm, index));
	}
};

}
