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

#include "lunaanimator.h"

namespace luna2d{

//--------------------------------------------
// Base action for actions working with object
//--------------------------------------------
class LUNAObjectAction : public LUNAAction
{
protected:
	LUNAObjectAction(const LuaTable& tblObj, float time, void* easeFunc = nullptr);

protected:
	LuaTable tblObj;
	void* easeFunc = nullptr;
};

//-----------------------
// Moving animator action
//-----------------------
class LUNAActionMove : public LUNAObjectAction
{
public:
	LUNAActionMove(const LuaTable& tblObj, const glm::vec2& begin, const glm::vec2& end, float time);

private:
	LuaFunction fnSetPos = nil;
	glm::vec2 begin;
	glm::vec2 end;

public:
	virtual void OnUpdate();
};

}
