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
#include "sqtable.h"
#include "sqfunction.h"
#include "sqclassinfo.h"
#include "lunalog.h"
#include "lunafiles.h"
#include <cstdarg>
#include <cstdio>

using namespace luna2d;

// Default print function
void OnPrint(HSQUIRRELVM vm, const SQChar* str, ...)
{
	va_list va;
	va_start(va, str);

	int size = vsnprintf(nullptr, 0, str, va);
	std::string buf(size + 1, '\0');
	vsnprintf(&buf[0], size + 1, str, va);

	va_end(va);

	LUNAEngine::SharedLog()->InfoString(buf);
}

// Runtime error handler
SQInteger OnRuntimeError(HSQUIRRELVM vm)
{
	const SQChar* error = nullptr;

	if(sq_gettop(vm) >= 1)
	{
		if(SQ_SUCCEEDED(sq_getstring(vm, 2, &error))) LUNA_LOG("%s", error);
		else LUNA_LOG("Unknown runtime error");

		sqstd_printcallstack(vm);
	}

	return SQ_ERROR;
}

// Compile error handler
void OnComplileError(HSQUIRRELVM vm, const SQChar *desc, const SQChar *source, SQInteger line, SQInteger column)
{
	LUNA_LOG("%s:%d:%d: %s", source, line, column, desc);
}


SqVm::SqVm() :
	vm(sq_open(SQUIRREL_STACK_SIZE))
{
	// Initialize squiirel std lib
	/*sqstd_register_iolib(vm);
	sqstd_register_bloblib(vm);
	sqstd_register_mathlib(vm);
	sqstd_register_systemlib(vm);
	sqstd_register_stringlib(vm);*/

	// Set default print function
	sq_setprintfunc(vm, &OnPrint, &OnPrint);

	// Set error handlers
	sq_newclosure(vm, OnRuntimeError, 0);
	sq_seterrorhandler(vm);
	sq_setcompilererrorhandler(vm, OnComplileError);

	// Create classes table in registry table
	GetRegistryTable().NewSlot(SQ_CLASSES_TABLE, SqTable(this), true);

	// Attach wrapper instance to squirrel VM
	sq_setforeignptr(vm, static_cast<SQUserPointer>(this));

	// Bind "require" function
	GetRootTable().NewSlot("require", SqFunction(this, this, &SqVm::Require));
}

SqVm::~SqVm()
{
	sq_close(vm);
	SqTypeTags::Reset();
}

void SqVm::Require(const std::string& file)
{
	std::string filename = file + ".nut";

	// Get path to script file where was called "require" function
	SQStackInfos info;
	sq_stackinfos(vm, 1, &info);

	// Try load file by relative path
	if(info.source)
	{
		std::string relativePath = LUNAEngine::SharedFiles()->GetParentPath(info.source) + "/" + filename;
		if(loadedFiles.count(relativePath) == 1) return;
		if(DoFile(relativePath))
		{
			 loadedFiles.insert(relativePath);
			 return;
		}
	}

	// Try load file by global path
	std::string globalPath = SCRIPTS_PATH + filename;
	if(loadedFiles.count(globalPath) == 1) return;
	if(DoFile(globalPath))
	{
		 loadedFiles.insert(globalPath);
		 return;
	}

	LUNA_LOGE("Cannot load file \"%s\", file not found", filename.c_str());
}

HSQUIRRELVM SqVm::GetVm() const
{
	return vm;
}

bool SqVm::DoString(const std::string& str, const std::string& sourceName)
{
	if(SQ_FAILED(sq_compilebuffer(vm, str.c_str(), str.size(), sourceName.c_str(), true))) return false;
	sq_pushroottable(vm);
	return SQ_SUCCEEDED(sq_call(vm, 1, false, true));
}

bool SqVm::DoFile(const std::string& filename)
{
	std::string buffer = LUNAEngine::SharedFiles()->ReadFileToString(filename);
	if(buffer.empty()) return false;

	return DoString(buffer, filename);
}

SqTable SqVm::GetRootTable() const
{
	sq_pushroottable(vm);
	SqScopedPop pop(vm, 1);
	return SqStack<SqTable>::Get(vm, -1);
}

SqTable SqVm::GetRegistryTable() const
{
	sq_pushregistrytable(vm);
	SqScopedPop pop(vm, 1);
	return SqStack<SqTable>::Get(vm, -1);
}

SqVm::operator HSQUIRRELVM() const
{
	return vm;
}

SqVm* SqVm::FromVm(HSQUIRRELVM vm)
{
	return static_cast<SqVm*>(sq_getforeignptr(vm));
}
