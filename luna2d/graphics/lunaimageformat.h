//-----------------------------------------------
// luna2d engine
// lunaimageformat.h - Interface for image format
// Copyright 2014 Stepan Prokofjev
//-----------------------------------------------

#pragma once

#include <stddef.h> // For size_t

namespace luna2d{

//---------------------------
// Color type of image format
//---------------------------
enum class LUNAColorType
{
	RGB = 1,
	RGBA = 2,
};

class LUNAImageFormat
{
public:
	virtual ~LUNAImageFormat() {}

public:
	// Decode raw image data to uncompressed bitmap data
	// Params:
	// "inData" - Input image data buffer
	// "inSize" - Size of input buffer
	// "outData" - Output decoded data buffer
	// "outSize" - Size of output buffer
	// "outWidth" - Output width of image
	// "outHeight" - Output height of image
	// "outColorType" - Output color type of image
	virtual bool Decode(unsigned char *inData, size_t inSize,
		unsigned char **outData, size_t *outSize, int *outWidth, int *outHeight, LUNAColorType *outColorType) = 0;
};

}
