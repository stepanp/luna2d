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

//-------------------------------------------
// Base class for actions working with object
//-------------------------------------------
LUNAObjectAction::LUNAObjectAction(const LuaTable& params) : LUNAAction(params)
{
	obj = params.GetField<LuaInstance>("obj");
	if(!obj) LUNA_LOGE("Object for animator action \"%s\" must be not nil", params.GetString("action").c_str());
}

//-----------------------
// Moving animator action
//-----------------------
LUNAActionMove::LUNAActionMove(const LuaTable& params) : LUNAObjectAction(params)
{
	// Get current pos of object if "beginValue" is nil
	if(!params.HasField("beginValue") && obj && obj.HasField("getPos")) begin = obj.CallMethod<glm::vec2>("getPos");

	else begin = params.GetField<glm::vec2>("beginValue");
	end = params.GetField<glm::vec2>("endValue");

	if(obj && !obj.HasField("setPos")) LUNA_LOGE("Object for action \"move\" must have \"setPos\" method");
}

void luna2d::LUNAActionMove::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	float x = math::Lerp(begin.x, end.x, GetPercent());
	float y = math::Lerp(begin.y, end.y, GetPercent());
	obj.CallMethodVoid("setPos", x, y);
}
