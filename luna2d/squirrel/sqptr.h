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

#include "sqstack.h"

namespace luna2d{

template<typename T>
class SqPtr
{
	union SqPtrType
	{
		std::shared_ptr<T> shared;
		std::weak_ptr<T> weak;
	};

public:
	SqPtr(const std::shared_ptr<T>& ptr) : isWeak(false), type(ptr) {}
	SqPtr(const std::weak_ptr<T>& ptr) : isWeak(true), type(ptr) {}

private:
	bool isWeak;
	SqPtrType type;

public:
	std::shared_ptr<T> ToShared()
	{
		if(isWeak) return nullptr; // Don't allow implicit casting from weak_ptr to shared_ptr
		else return type.shared;
	}

	std::weak_ptr<T> ToWeak()
	{
		if(isWeak) return type.weak;
		else return type.shared;
	}
};

}
