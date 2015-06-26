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
#include "lunalog.h"

#include <cstdarg>
#include <cstdio>

using namespace luna2d;

void TestPrintFunc(HSQUIRRELVM vm, const SQChar* str, ...)
{
	va_list args;
	va_start(args, str);
	const int size = vsnprintf(nullptr, 0, str, args);
	va_end(args);
	va_start(args, str);
	std::string chars(size + 1, 0);
	vsnprintf(&chars[0], size + 1, str, args);
	va_end(args);

	LUNAEngine::SharedLog()->InfoString(chars);
}

// Runtime error handler
SQInteger OnRuntimeError(HSQUIRRELVM vm)
{
	//LUNA_LOG("%s:%d:%d: %s", source, line, column, desc);
	LUNA_LOG("Runtime error");

	return SQ_ERROR;;
}

// Compile error handler
void OnComplileError(HSQUIRRELVM vm, const SQChar *desc, const SQChar *source, SQInteger line, SQInteger column)
{
	LUNA_LOG("%s:%d:%d: %s", source, line, column, desc);
}


SqVm::SqVm() :
	vm(::sq_open(SQUIRREL_STACK_SIZE))
{
	// Initialize squiirel std lib
	sqstd_register_mathlib(vm);
	sqstd_register_stringlib(vm);
	sqstd_register_bloblib(vm);

	// Set error handlers
	sq_newclosure(vm, OnRuntimeError, 0);
	sq_seterrorhandler(vm);
	sq_setcompilererrorhandler(vm, OnComplileError);

	sq_setprintfunc(vm, &TestPrintFunc, &TestPrintFunc);
}

SqVm::~SqVm()
{
	sq_close(vm);
}

bool SqVm::DoString(const std::string& str)
{
	if(SQ_FAILED(sq_compilebuffer(vm, str.c_str(), str.size(), "", true))) return false;
	sq_pushroottable(vm);
	return SQ_SUCCEEDED(sq_call(vm, 1, false, true));
}

bool SqVm::DoFile(const std::string& filename)
{

}

SqVm::operator HSQUIRRELVM()
{
	return vm;
}
