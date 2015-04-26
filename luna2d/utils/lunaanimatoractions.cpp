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

#include "lunaanimatoractions.h"
#include "lunamath.h"

using namespace luna2d;

//--------------------------------------------
// Base action for actions working with object
//--------------------------------------------
LUNAObjectAction::LUNAObjectAction(const LuaTable& tblObj, float time, void* easeFunc) :
	LUNAAction(time),
	tblObj(tblObj),
	easeFunc(easeFunc)
{
}

//-----------------------
// Moving animator action
//-----------------------
LUNAActionMove::LUNAActionMove(const LuaTable& tblObj, const glm::vec2& begin, const glm::vec2& end, float time) :
	LUNAObjectAction(tblObj, time),
	begin(begin),
	end(end)
{
	fnSetPos = tblObj.GetFunction("setPos");
	if(!fnSetPos) LUNA_LOGE("Object for action \"move\" must have \"setPos\" method");
}

void luna2d::LUNAActionMove::OnUpdate()
{
	float x = math::Lerp(begin.x, begin.x, time);
	float y = math::Lerp(begin.y, begin.y, time);

	if(fnSetPos) fnSetPos.CallVoid(x, y);
}
