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

#include "lunabindings.h"
#include "lunaengine.h"
#include "lunajsonutils.h"
#include "lunaaudio.h"
#include "lunasizes.h"
#include "lunaplatformutils.h"
#include "lunaprefs.h"
#include "lunatimer.h"
#include "lunaanimator.h"
#include "math/lunamath.h"
#include "math/lunavector2.h"
#include "math/lunarect.h"
#include "math/lunaintersect.h"
#include "math/lunasplines.h"
#include "math/lunaeasing.h"
#include "math/lunabounds.h"

using namespace luna2d;

// Bind "luna.log" module
static void BindLog(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblLog(lua);
	tblLuna.SetField("log", tblLog);

	// Register "info", "warning", "error" functions in "luna.log" table
	tblLog.SetField("info", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::InfoString));
	tblLog.SetField("warning", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::WarningString));
	tblLog.SetField("error", LuaFunction(lua, LUNAEngine::SharedLog(), &LUNALog::ErrorString));

	// Use "__call" metamethod of "luna.log" table as alias for "info" function
	// i.e. luna.log("message") = luna.log.info("message")
	LuaTable metaLog(lua);
	std::function<void(LuaNil, const std::string&)> proxy = [](LuaNil, const std::string& msg)
	{
		LUNAEngine::SharedLog()->InfoString(msg);
	};
	metaLog.SetField("__call", LuaFunction(lua, proxy));
	tblLog.SetMetatable(metaLog);

	// Register "table" function
	// "table" function for simplicity implemented as embedded script
	// SEE: "lua/scripts/logtable.lua.h"
	lua->DoString(LUNA_LUA_LOG_TABLE);
}

// Bind "luna.utilss" module
static void BindUtils(LuaScript* lua, LuaTable& tblLuna)
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

// Bind extension for standard lua "math" module
static void BindMath(LuaScript* lua, LuaTable& tblLuna)
{
	// Register additional math functions in standard lua "math" module
	LuaTable tblMath = lua->GetGlobalTable().GetTable("math");

	tblMath.SetField("frandom", LuaFunction(lua, &math::RandomFloat));
	tblMath.SetField("avg", LuaFunction(lua, &math::Avg));
	tblMath.SetField("lerp", LuaFunction(lua, &math::Lerp));
	tblMath.SetField("round", LuaFunction(lua, &std::roundf));

	// Register metatable for "Vec2"
	luaL_newmetatable(*lua, VECTOR2_METATABLE_NAME.c_str());
	LuaTable tblVector2(*lua, luaL_ref(*lua, LUA_REGISTRYINDEX));
	tblVector2.SetField("__index", tblVector2);
	tblVector2.SetField("add", &LuaVec2::Add);
	tblVector2.SetField("sub", &LuaVec2::Sub);
	tblVector2.SetField("scale", &LuaVec2::Scale);
	tblVector2.SetField("rotate", &LuaVec2::Rotate);
	tblVector2.SetField("nor", &LuaVec2::Nor);
	tblVector2.SetField("perp", &LuaVec2::Perp);
	tblVector2.SetField("angle", &LuaVec2::Angle);
	tblVector2.SetField("len", &LuaVec2::Len);
	tblVector2.SetField("lenSqr", &LuaVec2::LenSqr);
	tblVector2.SetField("dist", &LuaVec2::Dist);
	tblVector2.SetField("distSqr", &LuaVec2::DistSqr);
	tblVector2.SetField("cross", &LuaVec2::Cross);
	tblVector2.SetField("dot", &LuaVec2::Dot);
	tblVector2.SetField("copy", &LuaVec2::Copy);
	tblMath.SetField("Vec2", tblVector2);

	LuaTable metaVector2(lua);
	metaVector2.SetField("__call", &LuaVec2::Construct);
	tblVector2.SetMetatable(metaVector2);

	// Register constructor for "Rect"
	tblMath.SetField("Rect", LUNARect::LuaConstruct);

	// Bind base bounds
	LuaClass<LUNABounds> clsBounds(lua);
	clsBounds.SetMethod("getX", &LUNABounds::GetX);
	clsBounds.SetMethod("getY", &LUNABounds::GetY);
	clsBounds.SetMethod("setX", &LUNABounds::SetX);
	clsBounds.SetMethod("setY", &LUNABounds::SetY);
	clsBounds.SetMethod("getPos", &LUNABounds::GetPos);
	clsBounds.SetMethod("setPos", &LUNABounds::SetPos);
	clsBounds.SetMethod("getBoundingBox", &LUNABounds::GetBoundingBox);
	clsBounds.SetMethod("isIntersect", &LUNABounds::IsIntersect);
	clsBounds.SetMethod("getOriginX", &LUNABounds::GetOriginX);
	clsBounds.SetMethod("getOriginY", &LUNABounds::GetOriginY);
	clsBounds.SetMethod("setOriginX", &LUNABounds::SetOriginX);
	clsBounds.SetMethod("setOriginY", &LUNABounds::SetOriginY);
	clsBounds.SetMethod("getOrigin", &LUNABounds::GetOrigin);
	clsBounds.SetMethod("setOrigin", &LUNABounds::SetOrigin);
	clsBounds.SetMethod("setOriginToCenter", &LUNABounds::SetOriginToCenter);
	clsBounds.SetMethod("getScaleX", &LUNABounds::GetScaleX);
	clsBounds.SetMethod("getScaleY", &LUNABounds::GetScaleY);
	clsBounds.SetMethod("setScaleX", &LUNABounds::SetScaleX);
	clsBounds.SetMethod("setScaleY", &LUNABounds::SetScaleY);
	clsBounds.SetMethod("setScale", &LUNABounds::SetScale);

	// Bind AABB bounds
	LuaClass<LUNAAABBBounds> clsAABBBounds(lua);
	clsAABBBounds.SetConstructor<float, float>();
	clsAABBBounds.SetMethod("getWidth", &LUNAAABBBounds::GetWidth);
	clsAABBBounds.SetMethod("getHeight", &LUNAAABBBounds::GetHeight);
	clsAABBBounds.SetMethod("setWidth", &LUNAAABBBounds::SetWidth);
	clsAABBBounds.SetMethod("setHeight", &LUNAAABBBounds::SetHeight);
	clsAABBBounds.SetMethod("setSize", &LUNAAABBBounds::SetSize);
	tblMath.SetField("AABBBounds", clsAABBBounds);
	tblMath.SetField("RectBounds", clsAABBBounds); // RectBounds is alias for AABBBounds

	// Bind polygon bounds
	LuaClass<LUNAPolygonBounds> clsPolygonBounds(lua);
	clsPolygonBounds.SetConstructor<const std::vector<glm::vec2>&>();
	clsPolygonBounds.SetMethod("getVertexes", &LUNAPolygonBounds::GetVertexes);
	clsPolygonBounds.SetMethod("setVertexes", &LUNAPolygonBounds::SetVertexes);
	clsPolygonBounds.SetMethod("getAngle", &LUNAPolygonBounds::GetAngle);
	clsPolygonBounds.SetMethod("setAngle", &LUNAPolygonBounds::SetAngle);
	tblMath.SetField("PolygonBounds", clsPolygonBounds);

	// Set "luna.math" as alias for standard "math" module
	tblLuna.SetField("math", tblMath);
}

// Bind "luna.intersect" module
static void BindIntersect(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblIntersect(lua);
	tblLuna.SetField("intersect", tblIntersect);

	tblIntersect.SetField("pointInRectangle", LuaFunction(lua, &intersect::PointInRectangle));
	tblIntersect.SetField("pointInCircle", LuaFunction(lua, &intersect::PointInCircle));
	tblIntersect.SetField("pointInPolygon", LuaFunction(lua, &intersect::PointInPolygon));
	tblIntersect.SetField("rectangles", LuaFunction(lua, &intersect::Rectangles));
	tblIntersect.SetField("lines", LuaFunction(lua, &intersect::Lines));
	tblIntersect.SetField("lineCircle", LuaFunction(lua, &intersect::LineCircle));
	tblIntersect.SetField("pointBetweenLines", LuaFunction(lua, &intersect::PointBetweenLines));
}

// Bind "luna.splines" module
static void BindSplines(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblSplines(lua);
	tblLuna.SetField("splines", tblSplines);

	tblSplines.SetField("quadraticBSpline", LuaFunction(lua, &splines::QuadraticBSpline));
	tblSplines.SetField("cubicBezier", LuaFunction(lua, &splines::CubicBezier));
}

// Bind "luna.easing" module
static void BindEasing(LuaScript* lua, LuaTable& tblLuna)
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

	// Function like "cubic-bezier" in CSS for useful making custom easings
	std::function<float(float,float,float,float,float)> fnCubicBezier =
	[](float p1x, float p1y, float p2x, float p2y, float t) -> float
	{
		return splines::CubicBezier(glm::vec2(0, 0), glm::vec2(p1x, p1y), glm::vec2(p2x, p2y), glm::vec2(1, 1), t).y;
	};
	tblEasing.SetField("cubicBezier", LuaFunction(lua, fnCubicBezier));
}

// Bind "luna.platform" module
static void BindPlatform(LuaScript* lua, LuaTable& tblLuna)
{
	auto platformUtils = LUNAEngine::SharedPlatformUtils();

	LuaTable tblPlatform(lua);
	tblLuna.SetField("platform", tblPlatform);

	tblPlatform.SetField("name", LUNA_PLATFORM_STRING);
	tblPlatform.SetField("emulator", LUNA_PLATFORM == LUNA_PLATFORM_QT);
	tblPlatform.SetField("openUrl", LuaFunction(lua, platformUtils, &LUNAPlatformUtils::OpenUrl));
	tblPlatform.SetField("messageDialog", LuaFunction(lua, platformUtils, &LUNAPlatformUtils::MessageDialog));
	tblPlatform.SetField("confirmDialog", LuaFunction(lua, platformUtils, &LUNAPlatformUtils::ConfirmDialog));
}

// Bind "luna.audio" module
static void BindAudio(LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblAudio(lua);
	tblLuna.SetField("audio", tblAudio);

	tblAudio.SetField("isMusicPlaying", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::IsMusicPlaying));
	tblAudio.SetField("playMusic", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::PlayMusic));
	tblAudio.SetField("stopMusic", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::StopMusic));
	tblAudio.SetField("playSound", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::PlaySound));
	tblAudio.SetField("stopAllSounds", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::StopAllSounds));
	tblAudio.SetField("getMusicVolume", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::GetMusicVolume));
	tblAudio.SetField("setMusicVolume", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::SetMusicVolume));
	tblAudio.SetField("getSoundVolume", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::GetSoundVolume));
	tblAudio.SetField("setSoundVolume", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::SetSoundVolume));
	tblAudio.SetField("isMusicMuted", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::IsMusicMuted));
	tblAudio.SetField("muteMusic", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::MuteMusic));
	tblAudio.SetField("isSoundMuted", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::IsSoundMuted));
	tblAudio.SetField("muteSound", LuaFunction(lua, LUNAEngine::SharedAudio(), &LUNAAudio::MuteSound));

	// Bind audio source
	LuaClass<LUNAAudioSource> clsAudioSource(lua);
}

// Bind "luna.prefs" module
static void BindPrefs(LuaScript* lua, LuaTable& tblLuna)
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

		// Lua tables stored in prefs as serialized json string
		case LUNAPrefType::TABLE:
		{
			std::string jsonStr = prefs->GetString(name);
			std::string err;
			json11::Json json = json11::Json::parse(jsonStr, err);

			if(json == nullptr) return nil;

			return LuaAny(lua, Json2Lua(json));
		}
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

		// Lua tables serializes to json and stores in prefs as string
		case LUA_TTABLE:
		{
			const auto& json = Lua2Json(value.ToTable());
			prefs->SetString(name, json.dump());
			prefs->SetPrefType(name, LUNAPrefType::TABLE);
		}
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

// Bind "luna.config" module
static void BindConfig(LuaScript* lua, LuaTable& tblLuna)
{
	auto config = LUNAEngine::Shared()->GetConfig();
	tblLuna.SetField("config", Json2Lua(config->GetCustomValues()));
}

// Bind "luna.sizes" module
static void BindSizes(LuaScript* lua, LuaTable& tblLuna)
{
	auto sizes = LUNAEngine::SharedSizes();

	LuaTable tblSizes(lua);
	tblLuna.SetField("sizes", tblSizes);

	tblSizes.SetField("getResolutionName", LuaFunction(lua, sizes, &LUNASizes::GetResolutionSuffix));
	tblSizes.SetField("getAspectRatio", LuaFunction(lua, sizes, &LUNASizes::GetAspectRatio));
	tblSizes.SetField("getPhysicalScreenWidth", LuaFunction(lua, sizes, &LUNASizes::GetPhysicalScreenWidth));
	tblSizes.SetField("getPhysicalScreenHeight", LuaFunction(lua, sizes, &LUNASizes::GetPhysicalScreenHeight));
	tblSizes.SetField("getScreenWidth", LuaFunction(lua, sizes, &LUNASizes::GetScreenWidth));
	tblSizes.SetField("getScreenHeight", LuaFunction(lua, sizes, &LUNASizes::GetScreenHeight));
	tblSizes.SetField("getContentWidth", LuaFunction(lua, sizes, &LUNASizes::GetContentWidth));
	tblSizes.SetField("getContentHeight", LuaFunction(lua, sizes, &LUNASizes::GetContentHeight));
}

// Bind common classes and functions to lua
// Bindings for some subsystems(graphics, assets, etc.) declated in subsystem constructors
// SEE: "lunagraphics.cpp", "lunassets.cpp"
void luna2d::DoBindings()
{
	LuaScript* lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");

	BindLog(lua, tblLuna);
	BindUtils(lua, tblLuna);
	BindMath(lua, tblLuna);
	BindIntersect(lua, tblLuna);
	BindSplines(lua, tblLuna);
	BindEasing(lua, tblLuna);
	BindPlatform(lua, tblLuna);
	BindAudio(lua, tblLuna);
	BindPrefs(lua, tblLuna);
	BindConfig(lua, tblLuna);
	BindSizes(lua, tblLuna);
}
