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

#include "lunaengine.h"
#include "lunalog.h"
#include <squirrel.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <sqstdsystem.h>
#include <sqstdaux.h>
#include "squtils.h"

namespace luna2d{

const size_t SQUIRREL_STACK_SIZE = 1024;
const std::string SQ_CLASSES_REGISTRY = "_luna2d_classes";

class SqTable;

class SqVm
{
public:
	SqVm();
	~SqVm();

private:
	HSQUIRRELVM vm;

public:
	HSQUIRRELVM GetVm() const;
	bool DoString(const std::string& str, const std::string& sourceName = "");
	bool DoFile(const std::string& filename);
	SqTable GetRootTable() const;
	SqTable GetRegistryTable() const;

	operator HSQUIRRELVM() const;

	static SqVm* FromVm(HSQUIRRELVM vm); // Get wrapper instance from HSQUIRRELVM
};

}
