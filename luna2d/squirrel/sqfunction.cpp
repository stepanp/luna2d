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

#include "sqfunction.h"

using namespace luna2d;

SqFunction::SqFunction() : SqObject() {}
SqFunction::SqFunction(SqVm* vm) : SqObject(*vm) {}
SqFunction::SqFunction(HSQUIRRELVM vm) : SqObject(vm) {}
SqFunction::SqFunction(const SqFunction& fn) : SqObject(fn.GetRef()) {}
SqFunction::SqFunction(const std::shared_ptr<SqRef>& ref) : SqObject(ref) {}

SqFunction::SqFunction(SqVm* vm, SQFUNCTION func, int nparams) : SqFunction(vm)
{
	Bind(func, nparams);
}

// Bind native squirrel closure
void SqFunction::Bind(SQFUNCTION func, int nparams)
{
	HSQUIRRELVM vm = ref->GetVm();
	if(!vm) return;

	sq_newclosure(vm, func, 0);
	if(nparams != 0) sq_setparamscheck(vm, nparams, nullptr);

	ref = std::make_shared<SqRef>(vm, -1);
	sq_pop(vm, 1); // Pop function from stack
}

SqFunction& SqFunction::operator=(const SqFunction& fn)
{
	ref = fn.GetRef();
	return *this;
}
