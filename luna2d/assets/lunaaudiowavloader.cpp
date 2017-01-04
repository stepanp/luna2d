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

#include "lunaaudiowavloader.h"

using namespace luna2d;

struct LUNAWaveHeader
{
	uint8_t riff[4];
	uint32_t riffSize;
	uint8_t wave[4];
	uint8_t fmt[4];
	uint32_t fmtSize;
	uint16_t format;
	uint16_t channels;
	uint32_t samplesPerSec;
	uint32_t bytesPerSec;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	uint8_t data[4];
	uint32_t dataSize;
};

bool LUNAAudioWavLoader::Load(const std::string& filename)
{
	auto fileData = LUNAEngine::SharedFiles()->ReadFile(filename);

	LUNAWaveHeader header;
	memcpy(&header, fileData.data(), sizeof(LUNAWaveHeader));

	if(memcmp("RIFF", header.riff, 4) || memcmp("WAVE", header.wave, 4) ||
		memcmp("fmt ", header.fmt, 4) || memcmp("data", header.data, 4))
	{
		return false;
	}

	fileData.erase(fileData.begin(), fileData.begin() + sizeof(LUNAWaveHeader)); // Remove header from PCM data
	source = std::make_shared<LUNAAudioSource>(fileData, header.samplesPerSec, header.bitsPerSample, header.channels);

	return true;
}

void LUNAAudioWavLoader::PushToLua(const std::string& name, LuaTable& parentTable)
{
	parentTable.SetField(name, source, true);
}
