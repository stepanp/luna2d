//---------------------------------------------------
// luna2d engine
// lunaintersect.h - Functions for check intersectons
// Copyright 2014-2014 Stepan Prokofjev
//---------------------------------------------------

#pragma once

#include "lunamodule.h"

namespace luna2d{

class LUNAIntersect : public LUNAModule
{
public:
	virtual void Load(LuaScript *lua);

	bool PointInRectangle(const LuaTable& point, const LuaTable& rect); // Check for point insinde in rectangle
	bool Rectangles(const LuaTable& rect1, const LuaTable& rect2); // Check intersection between two rectangles
	bool Lines(const LuaTable& line1, const LuaTable& line2); // Check intersection between two lines
	bool LineCircle(const LuaTable& line, const LuaTable& point, float r); // Check intersection between line and circle
	LuaTable PointBetweenLines(const LuaTable& line1, const LuaTable& line2); // Get intersection point between two lines
};

}
