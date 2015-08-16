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

#include "lunabindings.h"
#include "lunaengine.h"
#include "lunasquirrel.h"
#include "lunalua.h"
#include "lunaprefs.h"
#include "lunatimer.h"
#include "lunaanimator.h"
#include "math/lunamath.h"
#include "math/lunavec2.h"
#include "math/lunavector2.h"
#include "math/lunarect.h"
#include "math/lunaintersect.h"
#include "math/lunasplines.h"
#include "math/lunaeasing.h"

using namespace luna2d;

// Bind "luna.log" module
void BindLog(SqVm* sq, SqTable& tblLuna)
{
	SqTable tblLog(sq);
	tblLuna.NewSlot("log", tblLog);

	// Register "info", "warning", "error" functions in "luna.log" table
	tblLog.NewSlot("info", SqFunction(sq, LUNAEngine::SharedLog(), &LUNALog::InfoString));
	tblLog.NewSlot("warning", SqFunction(sq, LUNAEngine::SharedLog(), &LUNALog::WarningString));
	tblLog.NewSlot("error", SqFunction(sq, LUNAEngine::SharedLog(), &LUNALog::ErrorString));

	// Use "__call" metamethod of "luna.log" table as alias for "info" function
	// i.e. luna.log("message") = luna.log.info("message")
	SqTable tblDelegate(sq);
	std::function<void(std::nullptr_t, const std::string&)> proxy = [](std::nullptr_t, const std::string& msg)
	{
		LUNAEngine::SharedLog()->InfoString(msg);
	};
	tblDelegate.NewSlot("_call", SqFunction(sq, proxy));
	tblLog.SetDelegate(tblDelegate);
}

// Bind "luna.utilss" module
void BindUtils(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblUtils(lua);
	tblLuna.SetField("utils", tblUtils);

	// Register "ChanceTable" class
	lua->DoString(LUNA_CHANCE_TABLE);

	// Register timer
	LuaClass<LUNATimer> clsTimer(lua);
	clsTimer.SetConstructor<float, const LuaFunction&, bool>();
	clsTimer.SetMethod("isLoop", &LUNATimer::IsLoop);
	clsTimer.SetMethod("setLoop", &LUNATimer::SetLoop);
	clsTimer.SetMethod("getTotalTime", &LUNATimer::GetTotalTime);
	clsTimer.SetMethod("getRemainingTime", &LUNATimer::GetRemainingTime);
	clsTimer.SetMethod("setTime", &LUNATimer::SetTime);
	clsTimer.SetMethod("getCallback", &LUNATimer::GetCallback);
	clsTimer.SetMethod("setCallback", &LUNATimer::SetCallback);
	clsTimer.SetMethod("isRunning", &LUNATimer::IsRunning);
	clsTimer.SetMethod("start", &LUNATimer::Start);
	clsTimer.SetMethod("pause", &LUNATimer::Pause);
	clsTimer.SetMethod("stop", &LUNATimer::Stop);
	clsTimer.SetMethod("update", &LUNATimer::Update);
	tblUtils.SetField("Timer", clsTimer);

	// Register animator
	LuaClass<LUNAAnimator> clsAnimator(lua);
	clsAnimator.SetConstructor<const LuaAny&>();
	clsAnimator.SetMethod("isLoop", &LUNAAnimator::IsLoop);
	clsAnimator.SetMethod("setLoop", &LUNAAnimator::SetLoop);
	clsAnimator.SetMethod("isRunning", &LUNAAnimator::IsRunning);
	clsAnimator.SetMethod("start", &LUNAAnimator::Start);
	clsAnimator.SetMethod("pause", &LUNAAnimator::Pause);
	clsAnimator.SetMethod("stop", &LUNAAnimator::Stop);
	clsAnimator.SetMethod("update", &LUNAAnimator::Update);
	tblUtils.SetField("Animator", clsAnimator);
}

// Bind "luna.math" module
void BindMath(SqVm* sq, SqTable& tblLuna)
{
	math::InitializeRandom();

	SqTable tblMath(sq);
	tblLuna.NewSlot("math", tblMath);

	// Squirrel standard library hasn't "min" and "max" functions
	std::function<float(float,float)> fnMax = [](float a, float b) -> float { return std::max(a, b); };
	std::function<float(float,float)> fnMin = [](float a, float b) -> float { return std::max(a, b); };
	tblMath.NewSlot("max", SqFunction(sq, fnMax));
	tblMath.NewSlot("min", SqFunction(sq, fnMin));

	tblMath.NewSlot("random", SqFunction(sq, &math::RandomFloat));
	tblMath.NewSlot("avg", SqFunction(sq, &math::Avg));
	tblMath.NewSlot("lerp", SqFunction(sq, &math::Lerp));

	// Bind math primitives
	BindVec2(sq, tblMath);
	BindRect(sq, tblMath);
}

// Bind "luna.intersect" module
void BindIntersect(SqVm* sq, SqTable& tblLuna)
{
	SqTable tblIntersect(sq);
	tblLuna.NewSlot("intersect", tblIntersect);

	tblIntersect.NewSlot("pointInRectangle", SqFunction(sq, &intersect::PointInRectangle));
	tblIntersect.NewSlot("pointInCircle", SqFunction(sq, &intersect::PointInCircle));
	tblIntersect.NewSlot("pointInPolygon", SqFunction(sq, &intersect::PointInPolygon));
	tblIntersect.NewSlot("rectangles", SqFunction(sq, &intersect::Rectangles));
	tblIntersect.NewSlot("lines", SqFunction(sq, &intersect::Lines));
	tblIntersect.NewSlot("lineCircle", SqFunction(sq, &intersect::LineCircle));
	tblIntersect.NewSlot("pointBetweenLines", SqFunction(sq, &intersect::PointBetweenLines));
}

// Bind "luna.splines" module
void BindSplines(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblSplines(lua);
	tblLuna.SetField("splines", tblSplines);

	tblSplines.SetField("quadraticBSpline", LuaFunction(lua, &splines::QuadraticBSpline));
}

// Bind "luna.easing" module
void BindEasing(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblEasing(lua);
	tblLuna.SetField("easing", tblEasing);

	// Bind all easing functions to lua as interpolation functions like: function(a, b, t)
	// Each function interpolate value between "a" and "b" by time "t" using own easing. "t" must be in range[0,1]
	for(auto& entry : EASINGS_MAP)
	{
		LUNAEasingFunc easing = entry.second;
		std::function<float(float, float, float)> easingFunc = [easing](float a, float b, float t) -> float
		{
			return math::EaseLerp(a, b, t, easing);
		};
		tblEasing.SetField(entry.first, LuaFunction(lua, easingFunc));
	}
}

// Bind "luna.platform" module
void BindPlatform(SqVm* vm, SqTable& tblLuna)
{
	tblLuna.NewSlot("platform", LUNA_PLATFORM_STRING);
}

// Bind "luna.prefs" module
void BindPrefs(LuaScript* lua, LuaTable& tblLuna)
{
	// "__index" metamethod handler
	// calls whenever acess to "luna.prefs" table like: "local value = luna.prefs.key"
	// Gets value from preferences
	// Type of value automaticaly depends by stored pref type
	std::function<LuaAny(LuaNil, const std::string&)> index =
		[lua](LuaNil, const std::string& name) -> LuaAny
	{
		if(name.empty()) return nil; // Ignore all keys except valid strings

		LUNAPrefs* prefs = LUNAEngine::SharedPrefs();
		LUNAPrefType type = prefs->GetPrefType(name);

		switch(type)
		{
		case LUNAPrefType::NONE:
			return nil;
		case LUNAPrefType::STRING:
			return LuaAny(lua, prefs->GetString(name));
		case LUNAPrefType::INT:
			return LuaAny(lua, prefs->GetInt(name));
		case LUNAPrefType::FLOAT:
			return LuaAny(lua, prefs->GetFloat(name));
		case LUNAPrefType::BOOL:
			return LuaAny(lua, prefs->GetBool(name));
		}

		return nil;
	};

	// "__newindex" metamethod handler
	// calls whenever acsess to "luna.prefs" table like: "luna.prefs.key = value"
	// Puts value to preferences subsytem and save pref type for automatic getting in "__index" method
	std::function<void(LuaNil, const std::string&, const LuaAny&)> newIndex =
		[](LuaNil, const std::string& name, const LuaAny& value)
	{
		if(name.empty()) return; // Ignore all keys except valid strings

		LUNAPrefs* prefs = LUNAEngine::SharedPrefs();
		int luaType = value.GetType();

		switch(luaType)
		{
		case LUA_TNIL:
			prefs->RemoveValue(name);
			prefs->RemovePrefType(name);
			break;
		case LUA_TSTRING:
			prefs->SetString(name, value.ToString());
			prefs->SetPrefType(name, LUNAPrefType::STRING);
			break;

		// All numbers in lua stored as floats
		// But for preferences in several cases (e.g. in Windows registry) it's not useful
		// So, check and store integer numbers as integers
		case LUA_TNUMBER:
		{
			float fValue = value.ToFloat();
			int iValue = (int)fValue;

			if(fValue == iValue)
			{
				prefs->SetInt(name, iValue);
				prefs->SetPrefType(name, LUNAPrefType::INT);
			}
			else
			{
				prefs->SetFloat(name, fValue);
				prefs->SetPrefType(name, LUNAPrefType::FLOAT);
			}

			break;
		}
		case LUA_TBOOLEAN:
			prefs->SetBool(name, value.ToBool());
			prefs->SetPrefType(name, LUNAPrefType::BOOL);
			break;
		default:
			break;
		}
	};

	LuaTable tblPrefs(lua);
	tblLuna.SetField("prefs", tblPrefs);

	LuaTable meta(lua);
	meta.SetField("__index", LuaFunction(lua, index));
	meta.SetField("__newindex", LuaFunction(lua, newIndex));
	tblPrefs.SetMetatable(meta);
}

// Bind common classes and functions to lua
// Bindings for some subsystems(graphics, assets, etc.) declated in subsystem constructors
// SEE: "lunagraphics.cpp", "lunassets.cpp"
void luna2d::DoBindings()
{
	SqVm* sq = LUNAEngine::SharedSquirrel();
	SqTable tblLuna = sq->GetRootTable().GetTable("luna");

	BindLog(sq, tblLuna);
	BindMath(sq, tblLuna);
	BindPlatform(sq, tblLuna);
	BindIntersect(sq, tblLuna);
}
