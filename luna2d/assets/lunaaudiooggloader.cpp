//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

#include "lunaaudiooggloader.h"
#include <vorbis/codec.h>
#include <vorbisfile.h>

using namespace luna2d;

struct LUNAOggData
{
	LUNAOggData(std::vector<unsigned char>& data) : data(data) {}

	std::vector<unsigned char>& data;
	size_t offset = 0;
};


static size_t ReadOgg(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	LUNAOggData* oggData = static_cast<LUNAOggData*>(datasource);
	size_t sizeToRead = size * nmemb;

	if(oggData->offset + sizeToRead > oggData->data.size())
	{
		sizeToRead = oggData->data.size() - oggData->offset;
	}

	memcpy(ptr, &oggData->data[oggData->offset], sizeToRead);
	oggData->offset += sizeToRead;

	return sizeToRead;
}

static int SeekOgg(void* datasource, ogg_int64_t offset, int whence)
{
	LUNAOggData* oggData = static_cast<LUNAOggData*>(datasource);

	switch (whence)
	{
	case SEEK_SET:
		oggData->offset = offset;
		break;
	case SEEK_CUR:
		oggData->offset += offset;
		break;
	case SEEK_END:
		oggData->offset = oggData->data.size() - offset;
		break;
	default:
		return -1;
	}

	if(oggData->offset > oggData->data.size()) return -1;
	return 0;
}

static long TellOgg(void* datasource)
{
	LUNAOggData* oggData = static_cast<LUNAOggData*>(datasource);
	return oggData->offset;
}

static int CloseOgg(void* datasource)
{
	return 0;
}


bool LUNAAudioOggLoader::Load(const std::string& filename)
{
	std::vector<unsigned char> fileData = LUNAEngine::SharedFiles()->ReadFile(filename);
	LUNAOggData oggData(fileData);

	OggVorbis_File oggFile;
	ov_callbacks oggCallbacks;
	oggCallbacks.close_func = CloseOgg;
	oggCallbacks.read_func = ReadOgg;
	oggCallbacks.seek_func = SeekOgg;
	oggCallbacks.tell_func = TellOgg;

	if(ov_open_callbacks(&oggData, &oggFile, nullptr, -1, oggCallbacks) < 0) return false;

	vorbis_info* oggInfo = ov_info(&oggFile, -1);
	long decodedSize = ov_pcm_total(&oggFile, -1) * oggInfo->channels * 2;
	long decodedTotal = 0;

	std::vector<unsigned char> decodedData;
	decodedData.resize(decodedSize);

	while(decodedTotal < decodedSize)
	{
		char* bufferPtr = reinterpret_cast<char*>(&decodedData[decodedTotal]);
		long result = ov_read(&oggFile, bufferPtr, decodedSize - decodedTotal, 0, 2, 1, nullptr);

		if(result < 0) return false;
		else if(result == 0) break;
		else decodedTotal += result;
	}

	source = std::make_shared<LUNAAudioSource>(decodedData, oggInfo->rate, OGG_SAMPLE_SIZE, oggInfo->channels);

	ov_clear(&oggFile);

	return true;
}

void LUNAAudioOggLoader::PushToLua(const std::string& name, luna2d::LuaTable& parentTable)
{
	parentTable.SetField(name, source, true);
}
