//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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
#include "lunacolor.h"
#include "lunaeasing.h"

namespace luna2d{

//-------------------------------------------
// Base class for actions working with object
//-------------------------------------------
class LUNAObjectAction : public LUNAAction
{
protected:
	LUNAObjectAction(const LuaTable& params);

protected:
	LuaInstance obj;
	std::function<float(float)> easing = &easing::Linear;

protected:
	// If given value isn't specifed in params table, try get current value from object
	// else just get value from params
	template<typename T>
	T TryGetCurrentValue(const LuaTable& params, const std::string& valueName, const std::string& methodName)
	{
		if(!params.HasField(valueName) && obj && obj.HasField(methodName)) return obj.CallMethod<T>(methodName);
		else return params.GetField<T>(valueName);
	}
};


//-----------------------
// Moving animator action
//-----------------------
class LUNAActionMove : public LUNAObjectAction
{
	enum class MoveMode
	{
		AXIS_X, // Move only by x-axis
		AXIS_Y, // Move only by y-axis
		AXIS_BOTH, // Move by both x and y axises
	};

public:
	LUNAActionMove(const LuaTable& params);

private:
	MoveMode mode;
	glm::vec2 begin;
	glm::vec2 end;

public:
	virtual void OnUpdate();
};


//-----------------------
// Fading animator action
//-----------------------
class LUNAActionFade : public LUNAObjectAction
{
public:
	LUNAActionFade(const LuaTable& params);

private:
	float begin;
	float end;

public:
	virtual void OnUpdate();
};


//------------------------
// Scaling animator action
//------------------------
class LUNAActionScale : public LUNAObjectAction
{
	enum class ScaleMode
	{
		AXIS_X, // Scale only by x-axis
		AXIS_Y, // Scale only by y-axis
		AXIS_BOTH, // Scale by both x and y axises
		COMMON // Scale by common value
	};

public:
	LUNAActionScale(const LuaTable& params);

private:
	ScaleMode mode;
	float beginX, endX;
	float beginY, endY;

public:
	virtual void OnUpdate();
};


//-------------------------
// Rotating animator action
//-------------------------
class LUNAActionRotate : public LUNAObjectAction
{
public:
	LUNAActionRotate(const LuaTable& params);

private:
	float begin;
	float end;

public:
	virtual void OnUpdate();
};


//-------------------------------
// Changing color animator action
//-------------------------------
class LUNAActionColor : public LUNAObjectAction
{
public:
	LUNAActionColor(const LuaTable& params);

private:
	LUNAColor begin;
	LUNAColor end;

public:
	virtual void OnUpdate();
};


//------------------------
// Shaking animator action
//------------------------
class LUNAActionShake : public LUNAObjectAction
{
public:
	LUNAActionShake(const LuaTable& params);

private:
	glm::vec2 beginAmp, endAmp;
	glm::vec2 beginFreq, endFreq;
	glm::vec2 pos;

private:
	glm::vec2 ReadValue(const std::string& name, const LuaTable& params);

public:
	virtual void OnUpdate();
};


//------------------------
// Waiting animator action
//------------------------
class LUNAActionWait : public LUNAAction
{
public:
	LUNAActionWait(const LuaTable& params) : LUNAAction(params) {}

public:
	virtual void OnUpdate() {}
};


//-----------------------
// Custom animator action
//-----------------------
class LUNAActionCustom : public LUNAAction
{
public:
	LUNAActionCustom(const LuaTable& params);

private:
	LuaFunction fnHandler;
	LUNAEasingFunc easing = easing::Linear;

public:
	virtual void OnUpdate();
};


//------------------------------
// Call function aniamtor action
//------------------------------
class LUNAActionFunction : public LUNAAction
{
public:
	LUNAActionFunction(const LuaFunction& fn);

private:
	LuaFunction fn;

public:
	virtual void OnUpdate();
};

}
