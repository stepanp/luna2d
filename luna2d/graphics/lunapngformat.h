//-----------------------------------
// luna2d engine
// lunapngformat.h - Png image format
// Copyright 2014-2014 Stepan Prokofjev
//-----------------------------------

#pragma once

#include "lunaimageformat.h"
#include <png.h>

namespace luna2d{

//-----------------------------------------------------
// Helper to pass data to "ReadPngFromBuffer" function
//-----------------------------------------------------
struct LUNAPngData
{
	unsigned char *data;
	size_t size;
	size_t offset;
};

class LUNAPngFormat : public LUNAImageFormat
{
public:
	// SEE: "LUNAImageFormat::Decode"
	virtual bool Decode(unsigned char *inData, size_t inSize,
			unsigned char **outData, size_t *outSize, int *outWidth, int *outHeight, LUNAColorType *outColorType);
};

}
