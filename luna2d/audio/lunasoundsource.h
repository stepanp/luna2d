//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

#include "lunaassets.h"
#include "lunalua.h"

namespace luna2d{

class LUNASoundSource : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNASoundSource)

public:
	virtual ~LUNASoundSource();

private:
	std::vector<unsigned char> data;
	int sampleRate = 0;
	int sampleSize = 0;
	int channelsCount = 0;

public:
	// Set data with swapping instead of copying buffer
	void SwapData(std::vector<unsigned char>& data, int sampleRate, int sampleSize, int channelsCount);

	// Set data with copying buffer
	void SetData(const std::vector<unsigned char>& data, int sampleRate, int sampleSize, int channelsCount);

	const std::vector<unsigned char>& GetData();
	int GetSampleRate();
	int GetSampleSize();
	int GetChannelsCount();
};

}
