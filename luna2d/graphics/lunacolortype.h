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
#include "lunastringenum.h"
#include "lunalua.h"
#include "lunagl.h"

namespace luna2d{

//---------------------------
// Color type of image format
//---------------------------
enum class LUNAColorType
{
	RGB,
	RGBA,
	ALPHA,
};

const LUNAStringEnum<LUNAColorType> COLOR_TYPE =
{
	"rgb",
	"rgba",
	"alpha",
};


template<>
struct LuaStack<LUNAColorType>
{
	static void Push(lua_State* luaVm, const LUNAColorType& type)
	{
		LuaStack<std::string>::Push(luaVm, COLOR_TYPE.FromEnum(type));
	}

	static LUNAColorType Pop(lua_State* luaVm, int index = -1)
	{
		auto strType = LuaStack<std::string>::Pop(luaVm, index);
		return COLOR_TYPE.FromString(strType, LUNAColorType::RGBA);
	}
};


// Get number of bytes per pixel for given color type
size_t GetBytesPerPixel(LUNAColorType colorType);

// Convert to GL color type
GLint ToGlColorType(LUNAColorType colorType);

}
