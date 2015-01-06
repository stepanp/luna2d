//--------------------------------
// luna2d engine
// lunacolor.h - Color
// Copyright 2014 Stepan Prokofjev
//--------------------------------

#include "lunacolor.h"

using namespace luna2d;

LUNAColor::LUNAColor(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

LUNAColor::LUNAColor()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 0;
}

// Convert value from byte format(0-255) to float format(0.0f-1.0f)
float LUNAColor::ByteToFloat(unsigned char value)
{
	return value / 255.0f;
}

// Values from 0 to 255
LUNAColor LUNAColor::Rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return LUNAColor(ByteToFloat(r), ByteToFloat(g), ByteToFloat(b), ByteToFloat(a));
}

// Values from 0.0f to 1.0f
LUNAColor LUNAColor::RgbFloat(float r, float g, float b, float a)
{
	return LUNAColor(r, g, b, a);
}

// From hex format(0xRRGGBB), alpha is optional
LUNAColor LUNAColor::Hex(int hex, float a)
{
	unsigned char byteA = (hex >> 16) & 0xFF;
	unsigned char byteB = (hex >> 8) & 0xFF;
	unsigned char byteC = hex & 0xFF;

	return LUNAColor(ByteToFloat(byteA), ByteToFloat(byteB), ByteToFloat(byteC), a);
}

// Initialize requently used colors
LUNAColor LUNAColor::WHITE = RgbFloat(1, 1, 1);
LUNAColor LUNAColor::BLACK = RgbFloat(0, 0, 0);
LUNAColor LUNAColor::GRAY = RgbFloat(0.5f, 0.5f, 0.5f);
LUNAColor LUNAColor::RED = RgbFloat(1, 0, 0);
LUNAColor LUNAColor::GREEN = RgbFloat(0, 1, 0);
LUNAColor LUNAColor::BLUE = RgbFloat(0, 0, 1);
LUNAColor LUNAColor::YELLOW = RgbFloat(1, 1, 0);
LUNAColor LUNAColor::CYAN = RgbFloat(0, 1, 1);
LUNAColor LUNAColor::PURPLE = RgbFloat(1, 0, 1);
