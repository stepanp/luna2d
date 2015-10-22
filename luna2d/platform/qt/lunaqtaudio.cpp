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

#include "lunaqtaudio.h"
#include "lunaqtaudiothread.h"

using namespace luna2d;

LUNAQtAudioPlayer::LUNAQtAudioPlayer()
{
}

void LUNAQtAudioPlayer::OnUsingChanged(bool inUse)
{
	this->inUse = inUse;
}

void LUNAQtAudioPlayer::OnPlayerStopped()
{
	emit playerStopped();
}

bool LUNAQtAudioPlayer::IsUsing()
{
	return inUse;
}

void LUNAQtAudioPlayer::SetSource(const std::shared_ptr<LUNAAudioSource>& source)
{
	LUNAQtAudio* audio = static_cast<LUNAQtAudio*>(LUNAEngine::SharedAudio());
	bufferId = source->GetBufferId();
	auto bufferData = audio->GetBuffer(bufferId);

	inUse = true;
	emit setSource(bufferData.get(), source->GetSampleRate(), source->GetSampleSize(), source->GetChannelsCount());
}

void LUNAQtAudioPlayer::SetLoop(bool loop)
{
	emit setLoop(loop);
}

void LUNAQtAudioPlayer::Play()
{
	emit play();
}

void LUNAQtAudioPlayer::Pause()
{
	emit pause();
}

void LUNAQtAudioPlayer::Stop()
{
	emit stop();
}

void LUNAQtAudioPlayer::Rewind()
{
	emit rewind();
}

void LUNAQtAudioPlayer::SetVolume(float volume)
{
	emit setVolume(volume);
}

void LUNAQtAudioPlayer::SetMute(bool mute)
{
	emit setMute(mute);
}


LUNAQtAudio::LUNAQtAudio()
{
	audioThread = new LUNAQtAudioThread(this);
	connect(audioThread, &LUNAQtAudioThread::finished, audioThread, &LUNAQtAudioThread::deleteLater);

	for(int i = 0; i < AUDIO_PLAYERS_COUNT_QT; i++)
	{
		auto player = std::make_shared<LUNAQtAudioPlayer>();
		audioThread->AttachPlayer(player.get());
		players.push_back(player);
	}

	audioThread->start();
}

LUNAQtAudio::~LUNAQtAudio()
{
	audioThread->quit();
}

void LUNAQtAudio::OnPlayersStopped(size_t bufferId)
{
	auto it = buffers.find(bufferId);
	if(it != buffers.end()) buffers.erase(bufferId);
}

// Get buffer data by given buffer id
std::shared_ptr<QByteArray> LUNAQtAudio::GetBuffer(size_t bufferId)
{
	auto it = buffers.find(bufferId);
	if(it == buffers.end()) return nullptr;
	return it->second;
}

// Create audio buffer from given audio data
// In case of success return id of created buffer, else return 0
size_t LUNAQtAudio::CreateBuffer(const std::vector<unsigned char>& data)
{
	auto bufferData = std::make_shared<QByteArray>(reinterpret_cast<const char*>(data.data()), data.size());
	buffers[++uniqueBufferId] = bufferData;
	return uniqueBufferId;
}

// Release buffer with given id
// All plyers using same buffer should be stopped
void LUNAQtAudio::ReleaseBuffer(size_t bufferId)
{
	if(buffers.count(bufferId) == 0) return;

	// Stop players using given buffer
	// Buffer will be released after stopping players in audio thread
	emit stopPlayers(bufferId);
}
