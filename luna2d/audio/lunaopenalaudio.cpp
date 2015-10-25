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

#include "lunaopenalaudio.h"

using namespace luna2d;

LUNAOpenAlAudioPlayer::LUNAOpenAlAudioPlayer()
{
	alGenSources(1, &soundId);
}

LUNAOpenAlAudioPlayer::~LUNAOpenAlAudioPlayer()
{
	Stop();
	if(alIsSource(soundId)) alDeleteSources(1, &soundId);
}

bool LUNAOpenAlAudioPlayer::IsUsing()
{
	if(isUsing)
	{
		ALint state;
		alGetSourcei(soundId, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	return isUsing;
}

void LUNAOpenAlAudioPlayer::SetSource(const std::shared_ptr<LUNAAudioSource>& source)
{
	bufferId = source->GetBufferId();
	alSourcei(soundId, AL_BUFFER, static_cast<unsigned int>(bufferId));

	isUsing = true;
}

void LUNAOpenAlAudioPlayer::SetLoop(bool loop)
{
	 alSourcei(soundId, AL_LOOPING, loop ? 1 : 0);
}

void LUNAOpenAlAudioPlayer::Play()
{
	alSourcePlay(soundId);
}

void LUNAOpenAlAudioPlayer::Pause()
{
	alSourcePause(soundId);
}

void LUNAOpenAlAudioPlayer::Stop()
{
	alSourceStop(soundId);
	isUsing = false;
}

void LUNAOpenAlAudioPlayer::Rewind()
{
	alSourceRewind(soundId);
}

void LUNAOpenAlAudioPlayer::SetVolume(float volume)
{
	alSourcef(soundId, AL_GAIN, volume);
}

void LUNAOpenAlAudioPlayer::SetMute(bool mute)
{

}


LUNAOpenAlAudio::LUNAOpenAlAudio()
{
	device = alcOpenDevice(nullptr);
	if(!device) LUNA_RETURN_ERR("Cannot open OpenAL audio device");

	context = alcCreateContext(device, nullptr);
	if(!context) LUNA_RETURN_ERR("Cannot open OpenAL audio context");

	alcMakeContextCurrent(context);

	for(int i = 0; i < SOUND_PLAYERS_COUNT_AL; i++) players.push_back(std::make_shared<LUNAOpenAlAudioPlayer>());
	musicPlayer = std::make_shared<LUNAOpenAlAudioPlayer>();
}

LUNAOpenAlAudio::~LUNAOpenAlAudio()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

// Create audio buffer from given audio data
// In case of success return id of created buffer, else return 0
size_t LUNAOpenAlAudio::CreateBuffer(const std::vector<unsigned char>& data,
	int sampleRate, int sampleSize, int channelsCount)
{
	unsigned int bufferId;
	alGenBuffers(1, &bufferId);

	ALenum format;
	if(channelsCount == 1 && sampleSize == 8) format = AL_FORMAT_MONO8;
	else if(channelsCount == 1 && sampleSize == 16) format = AL_FORMAT_MONO16;
	else if(channelsCount == 2 && sampleSize == 8) format = AL_FORMAT_STEREO8;
	else if(channelsCount == 2 && sampleSize == 16) format = AL_FORMAT_STEREO16;

	alBufferData(bufferId, format, data.data(), data.size(), sampleRate);

	return bufferId;
}

// Release buffer with given id
// All plyers using same buffer should be stopped
void LUNAOpenAlAudio::ReleaseBuffer(size_t bufferId)
{
	for(auto& player : players) player->Stop();
	musicPlayer->Stop();

	unsigned int id = static_cast<unsigned int>(bufferId);
	alDeleteBuffers(1, &id);
}
