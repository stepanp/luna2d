//-------------------------------------------
// luna2d engine
// lunaimage.cpp - Class for work with images
// Copyright 2014 Stepan Prokofjev
//-------------------------------------------

#include "lunaimage.h"
#include "lunaengine.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunatexture.h"

using namespace luna2d;

LUNAImage::LUNAImage()
{
	data = nullptr;
	width = 0;
	height = 0;
	dataSize = 0;
	colorType = LUNAColorType::RGBA;
}

LUNAImage::~LUNAImage()
{
	delete[] data;
}

unsigned char* LUNAImage::GetData() const
{
	return data;
}

int LUNAImage::GetWidth() const
{
	return width;
}

int LUNAImage::GetHeight() const
{
	return height;
}

LUNAColorType LUNAImage::GetColorType() const
{
	return colorType;
}

bool LUNAImage::Load(const std::string& filename, LUNAImageFormat *format, LUNAFileLocation location)
{
	if(!format) return false;

	// Read data form file
	std::vector<unsigned char> fileData = LUNAEngine::SharedFiles()->ReadFile(filename, location);
	if(fileData.empty()) return false;

	return format->Decode(fileData.data(), fileData.size(), &data, &dataSize, &width, &height, &colorType);
}
