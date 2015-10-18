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

using namespace luna2d;

void LUNAQtAudioPlayer::SetSource(const std::weak_ptr<LUNAAudioSource>& source)
{

}

void LUNAQtAudioPlayer::Play()
{

}

void LUNAQtAudioPlayer::Pause()
{

}

void LUNAQtAudioPlayer::Stop()
{

}

void LUNAQtAudioPlayer::SetVolume(float volume)
{

}

void LUNAQtAudioPlayer::SetMute(bool mute)
{

}

// Create audio buffer from given audio data
// In case of success return id of created buffer, else return 0
size_t LUNAQtAudio::CreateBuffer(const std::vector<unsigned char>& data)
{
	auto buffer = std::make_shared<QBuffer>();
	buffer->setData(reinterpret_cast<const char*>(data.data()), data.size());

	buffers[uniqueBufferId++] = buffer;

	return uniqueBufferId;
}

// Release buffer with given id
// All plyers using same buffer should be stopped
void LUNAQtAudio::ReleaseBuffer(size_t bufferId)
{
	auto bufferIt = buffers.find(bufferId);
	if(bufferIt == buffers.end()) return;

	buffers.erase(bufferId);
}

// Play sound from given source
void LUNAQtAudio::PlaySound(const std::weak_ptr<LUNAAudioSource>& source)
{
	if(source.expired()) LUNA_RETURN_ERR("Attempt to play invalid audio source");

	auto player = std::make_shared<LUNAQtAudioPlayer>();
	player->SetSource(source);
	player->Play();

	players.push_back(player);
}
