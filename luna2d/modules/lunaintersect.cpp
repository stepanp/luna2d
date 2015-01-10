//-----------------------------------------------------
// luna2d engine
// lunaintersect.cpp - Functions for check intersectons
// Copyright 2014-2014 Stepan Prokofjev
//-----------------------------------------------------

#include "lunaintersect.h"

using namespace luna2d;

void LUNAIntersect::Load(LuaScript *lua)
{
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblIntersect(lua);

	tblIntersect.SetField("pointInRectangle", LuaFunction(lua, this, &LUNAIntersect::PointInRectangle));
	tblIntersect.SetField("rectangles", LuaFunction(lua, this, &LUNAIntersect::Rectangles));
	tblIntersect.SetField("lines", LuaFunction(lua, this, &LUNAIntersect::Lines));
	tblIntersect.SetField("lineCircle", LuaFunction(lua, this, &LUNAIntersect::LineCircle));
	tblIntersect.SetField("pointBetweenLines", LuaFunction(lua, this, &LUNAIntersect::PointBetweenLines));

	tblLuna.SetField("intersect", tblIntersect);
}

// Check for point insinde in rectangle
bool LUNAIntersect::PointInRectangle(const LuaTable& point, const LuaTable& rect)
{
	float px = point.GetFloat("x");
	float py = point.GetFloat("y");

	float rx = rect.GetFloat("x");
	float ry = rect.GetFloat("y");
	float rwidth = rect.GetFloat("width");
	float rheight = rect.GetFloat("height");

	return px > rx && py > ry && px < rx + rwidth && py < ry + rheight;
}

// Check intersection between two rectangles
bool LUNAIntersect::Rectangles(const LuaTable& rect1, const LuaTable& rect2)
{
	float x1 = rect1.GetFloat("x");
	float y1 = rect1.GetFloat("y");
	float width1 = rect1.GetFloat("width");
	float height1 = rect1.GetFloat("height");

	float x2 = rect2.GetFloat("x");
	float y2 = rect2.GetFloat("y");
	float width2 = rect2.GetFloat("width");
	float height2 = rect2.GetFloat("height");

	return x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2;
}

// Check intersection between two lines
bool LUNAIntersect::Lines(const LuaTable& line1, const LuaTable& line2)
{
	float x1 = line1.GetFloat("x1");
	float y1 = line1.GetFloat("y1");
	float x2 = line1.GetFloat("x2");
	float y2 = line1.GetFloat("y2");

	float x3 = line2.GetFloat("x1");
	float y3 = line2.GetFloat("y1");
	float x4 = line2.GetFloat("x2");
	float y4 = line2.GetFloat("y2");

	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if(d == 0) return false;

	float yd = y1 - y3;
	float xd = x1 - x3;
	float ua = ((x4 - x3) * yd  - (y4 - y3) * xd) / d;
	if(ua < 0 || ua > 1) return false;

	float ub = ((x2 - x1) * yd  - (y2 - y1) * xd) / d;
	if(ub < 0 || ub > 1) return false;

	return true;
}

// Check intersection between line and circle
bool LUNAIntersect::LineCircle(const LuaTable& line, const LuaTable& point, float r)
{
	float x1 = line.GetFloat("x1");
	float y1 = line.GetFloat("y1");
	float x2 = line.GetFloat("x2");
	float y2 = line.GetFloat("y2");
	float cx = point.GetFloat("x");
	float cy = point.GetFloat("y");

	x1 -= cx;
	y1 -= cy;
	x2 -= cx;
	y2 -= cy;

	float dx = x2 - x1;
	float dy = y2 - y1;

	float a = dx * dx + dy * dy;
	float b = 2 * (x1 * dx + y1 * dy);
	float c = x1 * x1 + y1 * y1 - r * r;

	if(-b < 0) return c < 0;
	else if(-b < 2 * a) return (4 * a * c - b * b) < 0;

	return (a + b + c) < 0;
}

// Get intersection point between two lines
LuaTable LUNAIntersect::PointBetweenLines(const LuaTable& line1, const LuaTable& line2)
{
	float x1 = line1.GetFloat("x1");
	float y1 = line1.GetFloat("y1");
	float x2 = line1.GetFloat("x2");
	float y2 = line1.GetFloat("y2");

	float x3 = line2.GetFloat("x1");
	float y3 = line2.GetFloat("y1");
	float x4 = line2.GetFloat("x2");
	float y4 = line2.GetFloat("y2");

	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if(d == 0) return nil;

	float yd = y1 - y3;
	float xd = x1 - x3;
	float ua = ((x4 - x3) * yd  - (y4 - y3) * xd) / d;
	if(ua < 0 || ua > 1) return nil;

	float ub = ((x2 - x1) * yd  - (y2 - y1) * xd) / d;
	if(ub < 0 || ub > 1) return nil;

	float x = x1 + ua * (x2 - x1);
	float y = y1 + ua * (y2 - y1);

	LuaTable tblRet(LUNAEngine::SharedLua());
	tblRet.SetField("x", x);
	tblRet.SetField("y", y);
	return std::move(tblRet);
}
