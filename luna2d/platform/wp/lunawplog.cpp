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
#include "lunawstring.h"
#include <Windows.h>

using namespace luna2d;

// Print log message with given log level
void PrintLog(const std::wstring& level, const char* message, va_list va)
{
	int size = _vscprintf(message, va) + 1; // Get required buffer size for "vsprintf_s" 
	std::string buf;
	buf.resize(size);
	vsprintf_s(&buf[0], size, message, va);

	// Prtint log level
	if(!level.empty())
	{
		OutputDebugString(level.c_str());
		OutputDebugString(L":");
	}

	// Print log message
	OutputDebugString(ToWString(buf).c_str());
	OutputDebugString(L"\n");
}

// Log info
void LUNAWpLog::Info(const char* message, ...)
{
	va_list va;
	va_start(va, message);

	PrintLog(L"", message, va);

	va_end(va);
}

// Log warning
void LUNAWpLog::Warning(const char* message, ...)
{
	va_list va;
	va_start(va, message);

	PrintLog(L"Warning", message, va);

	va_end(va);
}

// Log error
void LUNAWpLog::Error(const char* message, ...)
{
	va_list va;
	va_start(va, message);
	
	PrintLog(L"Error", message, va);

	va_end(va);
}