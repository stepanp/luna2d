//-----------------------------------------
// luna2d engine
// lunaimage.h - Class for work with images
// Copyright 2014-2014 Stepan Prokofjev
//-----------------------------------------

#pragma once

#include "platform/lunafiles.h"
#include "lunapngformat.h"
#include <string>

namespace luna2d{

class LUNATexture;

class LUNAImage
{
public:
	LUNAImage();
	virtual ~LUNAImage();

private:
	unsigned char *data;
	size_t dataSize;
	int width, height;
	LUNAColorType colorType;

public:
	unsigned char* GetData() const;
	int GetWidth() const;
	int GetHeight() const;
	LUNAColorType GetColorType() const;
	bool Load(const std::string& filename, LUNAImageFormat *format, LUNAFileLocation location = LUNAFileLocation::ASSETS);
};

}
