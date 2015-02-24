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

#include "lunawplog.h"
#include <Windows.h>

using namespace luna2d;

// Print log message with given log level
void LUNAWpLog::PrintLog(const std::string& level, const char* message, va_list va)
{
	int size = _vscprintf(message, va) + 1; // Get required buffer size for "vsprintf_s" 
	std::string buf;
	buf.resize(size);
	vsprintf_s(&buf[0], size, message, va);

	buf[size - 1] = '\n'; // Replace extra '\0' char with new line char
	if(!level.empty()) buf.insert(0, level + ":");

	OutputDebugStringA(buf.c_str());
}

// Log info
void LUNAWpLog::Info(const char* message, ...)
{
	va_list va;
	va_start(va, message);

	PrintLog("", message, va);

	va_end(va);
}

// Log warning
void LUNAWpLog::Warning(const char* message, ...)
{
	va_list va;
	va_start(va, message);

	PrintLog("Warning", message, va);

	va_end(va);
}

// Log error
void LUNAWpLog::Error(const char* message, ...)
{
	va_list va;
	va_start(va, message);
	
	PrintLog("Error", message, va);

	va_end(va);
}