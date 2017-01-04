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

	// Get easing function if specifed. Default easing is linear
	LuaAny luaEasing = params.GetField<LuaAny>("easing");

	// Get easing function by name
	if(luaEasing.GetType() == LUA_TSTRING)
	{
		std::string easingName = luaEasing.ToString();
		if(EASINGS_MAP.count(easingName) == 1) easing = EASINGS_MAP.at(easingName);
		else LUNA_LOGE("Easing with name \"%s\" not found. Using default \"linear\" easing", easingName.c_str());
	}

	// Or use custom easing function
	else if(luaEasing.GetType() == LUA_TFUNCTION)
	{
		LuaFunction customEasing = luaEasing.ToFunction();
		easing = [customEasing](float t) -> float { return customEasing.Call<float>(t); };
	}
}

//-----------------------
// Moving animator action
//-----------------------
LUNAActionMove::LUNAActionMove(const LuaTable& params) : LUNAObjectAction(params)
{
	if(params.HasField("endValueX") && !params.HasField("endValueY")) mode = MoveMode::AXIS_X;
	else if(!params.HasField("endValueX") && params.HasField("endValueY")) mode = MoveMode::AXIS_Y;
	else mode = MoveMode::AXIS_BOTH;

	if(mode == MoveMode::AXIS_X)
	{
		begin.x = TryGetCurrentValue<float>(params, "beginValueX", "getX");
		end.x = params.GetFloat("endValueX");

		if(obj && !obj.HasField("setX")) LUNA_LOGE("Object for action \"move\" must have \"setX\" method");
	}

	if(mode == MoveMode::AXIS_Y)
	{
		begin.y = TryGetCurrentValue<float>(params, "beginValueY", "getY");
		end.y = params.GetFloat("endValueY");

		if(obj && !obj.HasField("setY")) LUNA_LOGE("Object for action \"move\" must have \"setY\" method");
	}

	if(mode == MoveMode::AXIS_BOTH)
	{
		if(params.HasField("endValue"))
		{
			begin = TryGetCurrentValue<glm::vec2>(params, "beginValue", "getPos");
			end = params.GetField<glm::vec2>("endValue");
		}
		else
		{
			begin.x = TryGetCurrentValue<float>(params, "beginValueX", "getX");
			end.x = params.GetFloat("endValueX");
			begin.y = TryGetCurrentValue<float>(params, "beginValueY", "getY");
			end.y = params.GetFloat("endValueY");
		}

		if(obj && !obj.HasField("setPos")) LUNA_LOGE("Object for action \"move\" must have \"setPos\" method");
	}
}

void luna2d::LUNAActionMove::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	if(mode == MoveMode::AXIS_X) obj.CallMethodVoid("setX", math::EaseLerp(begin.x, end.x, GetPercent(), easing));
	else if(mode == MoveMode::AXIS_Y) obj.CallMethodVoid("setY", math::EaseLerp(begin.y, end.y, GetPercent(), easing));
	else
	{
		float x = math::EaseLerp(begin.x, end.x, GetPercent(), easing);
		float y = math::EaseLerp(begin.y, end.y, GetPercent(), easing);
		obj.CallMethodVoid("setPos", x, y);
	}
}


//-----------------------
// Fading animator action
//-----------------------
LUNAActionFade::LUNAActionFade(const LuaTable& params) : LUNAObjectAction(params)
{
	begin = TryGetCurrentValue<float>(params, "beginValue", "getAlpha");
	end = params.GetFloat("endValue");

	if(obj && !obj.HasField("setAlpha")) LUNA_LOGE("Object for action \"fade\" must have \"setAlpha\" method");
}

void LUNAActionFade::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	float alpha = math::EaseLerp(begin, end, GetPercent(), easing);
	obj.CallMethodVoid("setAlpha", alpha);
}


//------------------------
// Scaling animator action
//------------------------
LUNAActionScale::LUNAActionScale(const LuaTable& params) : LUNAObjectAction(params)
{
	if(params.HasField("endValueX") && params.HasField("endValueY")) mode = ScaleMode::AXIS_BOTH;
	else if(params.HasField("endValueX") && !params.HasField("endValueY")) mode = ScaleMode::AXIS_X;
	else if(!params.HasField("endValueX") && params.HasField("endValueY")) mode = ScaleMode::AXIS_Y;
	else mode = ScaleMode::COMMON;

	if(mode == ScaleMode::COMMON)
	{
		beginX = TryGetCurrentValue<float>(params, "beginValue", "getScale");
		endX = params.GetFloat("endValue");

		if(obj && !obj.HasField("setScale")) LUNA_LOGE("Object for action \"scale\" must have \"setScale\" method");
	}

	else if(mode == ScaleMode::AXIS_X)
	{
		beginX = TryGetCurrentValue<float>(params, "beginValueX", "getScaleX");
		endX = params.GetFloat("endValueX");

		if(obj && !obj.HasField("setScaleX")) LUNA_LOGE("Object for action \"scale\" must have \"setScaleX\" method");
	}

	if(mode == ScaleMode::AXIS_Y)
	{
		beginY = TryGetCurrentValue<float>(params, "beginValueY", "getScaleY");
		endY = params.GetFloat("endValueY");

		if(obj && !obj.HasField("setScaleY")) LUNA_LOGE("Object for action \"scale\" must have \"setScaleY\" method");
	}

	if(mode == ScaleMode::AXIS_BOTH)
	{
		beginX = TryGetCurrentValue<float>(params, "beginValueX", "getScaleX");
		endX = params.GetFloat("endValueX");
		beginY = TryGetCurrentValue<float>(params, "beginValueY", "getScaleY");
		endY = params.GetFloat("endValueY");

		if(obj && (!obj.HasField("setScaleX") || !obj.HasField("setScaleY")))
		{
			LUNA_LOGE("Object for action \"scale\" must have \"setScaleX\" and \"setScaleY\" methods");
		}
	}
}

void LUNAActionScale::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	if(mode == ScaleMode::AXIS_X)
	{
		float scaleX = math::EaseLerp(beginX, endX, GetPercent(), easing);
		obj.CallMethodVoid("setScaleX", scaleX);
	}
	else if(mode == ScaleMode::AXIS_Y)
	{
		float scaleY = math::EaseLerp(beginY, endY, GetPercent(), easing);
		obj.CallMethodVoid("setScaleY", scaleY);
	}
	else if(mode == ScaleMode::AXIS_BOTH)
	{
		float scaleX = math::EaseLerp(beginX, endX, GetPercent(), easing);
		float scaleY = math::EaseLerp(beginY, endY, GetPercent(), easing);
		obj.CallMethodVoid("setScaleX", scaleX);
		obj.CallMethodVoid("setScaleY", scaleY);
	}
	else if(mode == ScaleMode::COMMON)
	{
		float scale = math::EaseLerp(beginX, endX, GetPercent(), easing);
		obj.CallMethodVoid("setScale", scale);
	}
}


//-------------------------
// Rotating animator action
//-------------------------
LUNAActionRotate::LUNAActionRotate(const LuaTable& params) : LUNAObjectAction(params)
{
	begin = TryGetCurrentValue<float>(params, "beginValue", "getAngle");
	end = params.GetFloat("endValue");

	if(obj && !obj.HasField("setAngle")) LUNA_LOGE("Object for action \"rotate\" must have \"setAngle\" method");
}

void LUNAActionRotate::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	float angle = math::EaseLerp(begin, end, GetPercent(), easing);
	obj.CallMethodVoid("setAngle", angle);
}


//-------------------------------
// Changing color animator action
//-------------------------------
LUNAActionColor::LUNAActionColor(const LuaTable& params) : LUNAObjectAction(params)
{
	begin = TryGetCurrentValue<LUNAColor>(params, "beginValue", "getColor");
	end = params.GetField<LUNAColor>("endValue");

	if(obj && !obj.HasField("setColor")) LUNA_LOGE("Object for action \"rotate\" must have \"setColor\" method");
}

void LUNAActionColor::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	int r = math::EaseLerp(begin.GetR(), end.GetR(), GetPercent(), easing);
	int g = math::EaseLerp(begin.GetG(), end.GetG(), GetPercent(), easing);
	int b = math::EaseLerp(begin.GetB(), end.GetB(), GetPercent(), easing);
	obj.CallMethodVoid("setColor", r, g, b);
}


//------------------------
// Shaking animator action
//------------------------
LUNAActionShake::LUNAActionShake(const LuaTable& params) : LUNAObjectAction(params)
{
	pos = obj.CallMethod<glm::vec2>("getPos");

	beginAmp = ReadValue("beginAmp", params);
	endAmp = ReadValue("endAmp", params);
	beginFreq = ReadValue("beginFreq", params);
	endFreq = ReadValue("endFreq", params);

	if(obj && (!obj.HasField("getPos") || !obj.HasField("setPos"))) LUNA_LOGE("Object for action \"shake\" must have \"getPos\" and \"setPos\" methods");
}

glm::vec2 LUNAActionShake::ReadValue(const std::string& name, const LuaTable& params)
{
	if(params.HasField(name))
	{
		float value = params.GetFloat(name);
		return glm::vec2(value, value);
	}
	else
	{
		return glm::vec2(params.GetFloat(name + "X"), params.GetFloat(name + "Y"));
	}
}

void LUNAActionShake::OnUpdate()
{
	if(!obj) LUNA_RETURN_ERR("Attempt to update invalid animator action");

	float ampX = math::EaseLerp(beginAmp.x, endAmp.x, GetPercent(), easing);
	float ampY = math::EaseLerp(beginAmp.y, endAmp.y, GetPercent(), easing);
	float freqX = math::EaseLerp(beginFreq.x, endFreq.x, GetPercent(), easing);
	float freqY = math::EaseLerp(beginFreq.y, endFreq.y, GetPercent(), easing);

	float dx = ampX * std::sin(GetTime() * freqX);
	float dy = ampY * std::sin(GetTime() * freqY);

	obj.CallMethodVoid("setPos", pos.x + dx, pos.y + dy);
}


//-----------------------
// Custom animator action
//-----------------------
LUNAActionCustom::LUNAActionCustom(const LuaTable& params) : LUNAAction(params)
{
	fnHandler = params.GetFunction("handler");
	if(!fnHandler) LUNA_LOGE("Animator action \"custom\" must have \"handler\" function in params");

	std::string easingName = params.GetString("easing");
	if(EASINGS_MAP.count(easingName) == 1) easing = EASINGS_MAP.at(easingName);
}

void LUNAActionCustom::OnUpdate()
{
	if(!fnHandler) LUNA_RETURN_ERR("Attempt to update invalid animator action");
	fnHandler.CallVoid(easing(GetPercent()), GetTime());
}


//------------------------------
// Call function aniamtor action
//------------------------------
LUNAActionFunction::LUNAActionFunction(const LuaFunction &fn) :
	LUNAAction(0),
	fn(fn)
{
}

void LUNAActionFunction::OnUpdate()
{
	fn.CallVoid();
}
