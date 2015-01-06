//--------------------------------
// luna2d engine
// lunacolor.h - Color
// Copyright 2014 Stepan Prokofjev
//--------------------------------

#pragma once

namespace luna2d{

struct LUNAColor
{
private:
	LUNAColor(float r, float g, float b, float a = 1.0f);

public:
	LUNAColor();

public:
	float r, g, b, a;

public:
	static float ByteToFloat(unsigned char value); // Convert value from byte format(0-255) to float format(0.0f-1.0f)

	static LUNAColor Rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255); // Values from 0 to 255
	static LUNAColor RgbFloat(float r, float g, float b, float a = 1.0f); // Values from 0.0f to 1.0f
	static LUNAColor Hex(int hex, float a = 1.0f); // From hex format(0xRRGGBB), alpha is optional

	// Frequently used colors
	static LUNAColor WHITE, BLACK, GRAY, RED, GREEN, BLUE, YELLOW, CYAN, PURPLE;
};

}
