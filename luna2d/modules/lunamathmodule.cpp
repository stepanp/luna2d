//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

#include "lunamathmodule.h"
#include "lunamathutils.h"
#include "lunavector2.h"

using namespace luna2d;

void LUNAMathModule::Load(LuaScript *lua)
{
	// Register additional math functions in standard lua "math" module
	LuaTable tblMath = lua->GetGlobalTable().GetTable("math");

	tblMath.SetField("frandom", LuaFunction(lua, &LUNAMathUtils::RandomFloat));
	tblMath.SetField("avg", LuaFunction(lua, &LUNAMathUtils::Avg));

	// Register metatable for vector2
	lua_State* luaVm = lua->GetLuaVm();
	luaL_newmetatable(luaVm, VECTOR2_METATABLE_NAME.c_str());

	LuaTable tblVector2(luaVm, luaL_ref(luaVm, LUA_REGISTRYINDEX));
	tblVector2.SetField("__index", tblVector2);
	tblVector2.SetField("add", &LuaVector2::Add);
	tblVector2.SetField("sub", &LuaVector2::Sub);
	tblVector2.SetField("scale", &LuaVector2::Scale);
	tblVector2.SetField("rotate", &LuaVector2::Rotate);
	tblVector2.SetField("nor", &LuaVector2::Nor);
	tblVector2.SetField("perp", &LuaVector2::Perp);
	tblVector2.SetField("angle", &LuaVector2::Angle);
	tblVector2.SetField("len", &LuaVector2::Len);
	tblVector2.SetField("lenSqr", &LuaVector2::LenSqr);
	tblVector2.SetField("dist", &LuaVector2::Dist);
	tblVector2.SetField("distSqr", &LuaVector2::DistSqr);
	tblVector2.SetField("cross", &LuaVector2::Cross);
	tblVector2.SetField("dot", &LuaVector2::Dot);
	tblVector2.SetField("copy", &LuaVector2::Copy);
	tblMath.SetField("Vector2", tblVector2);

	LuaTable metaVector2(lua);
	metaVector2.SetField("__call", &LuaVector2::Construct);
	tblVector2.SetMetatable(metaVector2);

	// Set "luna.math" as alias for standard "math" module
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	tblLuna.SetField("math", tblMath);
}
