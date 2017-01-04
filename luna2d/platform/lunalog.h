//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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

namespace luna2d{

//--------------
// Log interface
//--------------
class LUNALog
{
public:
	virtual ~LUNALog() {}

public:
	virtual void Info(const char* message, ...) = 0; // Log info
	virtual void Warning(const char* message, ...) = 0; // Log warning
	virtual void Error(const char* message, ...) = 0; // Log error

	void InfoString(const std::string& message);
	void WarningString(const std::string& message);
	void ErrorString(const std::string& message);
};

}

//-------------------
// Define short macro
//-------------------
#define LUNA_LOGI(...) luna2d::LUNAEngine::SharedLog()->Info(__VA_ARGS__)
#define LUNA_LOGW(...) luna2d::LUNAEngine::SharedLog()->Warning(__VA_ARGS__)
#define LUNA_LOGE(...) luna2d::LUNAEngine::SharedLog()->Error(__VA_ARGS__)
#define LUNA_LOG LUNA_LOGI
