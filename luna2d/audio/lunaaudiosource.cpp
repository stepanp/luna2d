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

#include "lunaaudiosource.h"
#include "lunaaudio.h"

using namespace luna2d;

LUNAAudioSource::LUNAAudioSource(std::vector<unsigned char>& data, int sampleRate, int sampleSize, int channelsCount) :
	sampleRate(sampleRate),
	sampleSize(sampleSize),
	channelsCount(channelsCount)
{
	alGenBuffers(1, &id);

	ALenum format;
	if(channelsCount == 1 && sampleSize == 8) format = AL_FORMAT_MONO8;
	else if(channelsCount == 1 && sampleSize == 16) format = AL_FORMAT_MONO16;
	else if(channelsCount == 2 && sampleSize == 8) format = AL_FORMAT_STEREO8;
	else if(channelsCount == 2 && sampleSize == 16) format = AL_FORMAT_STEREO16;

	alBufferData(id, format, data.data(), data.size(), sampleRate);
}

LUNAAudioSource::~LUNAAudioSource()
{
	LUNAEngine::SharedAudio()->StopPlayersWithSource(id);

	alDeleteBuffers(1, &id);
}

ALuint LUNAAudioSource::GetId()
{
	return id;
}

int LUNAAudioSource::GetSampleRate()
{
	return sampleRate;
}

int LUNAAudioSource::GetSampleSize()
{
	return sampleSize;
}

int LUNAAudioSource::GetChannelsCount()
{
	return channelsCount;
}
