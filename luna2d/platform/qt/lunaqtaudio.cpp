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
#include <QApplication>

using namespace luna2d;

LUNAQtAudioPlayer::LUNAQtAudioPlayer() :
	buffer(std::make_shared<QBuffer>())
{
	format.setSampleRate(0);
	format.setSampleSize(0);
	format.setChannelCount(0);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);
}

void LUNAQtAudioPlayer::OnStateChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::IdleState:
		output->stop();
		buffer->close();
		used = false;
	}
}

bool LUNAQtAudioPlayer::IsUsing()
{
	return used;
}

void LUNAQtAudioPlayer::SetSource(const std::shared_ptr<LUNAAudioSource>& source)
{
	LUNAQtAudio* audio = static_cast<LUNAQtAudio*>(LUNAEngine::SharedAudio());

	bufferId = source->GetBufferId();

	auto bufferData = audio->GetBuffer(bufferId);
	buffer->setBuffer(bufferData.get());
	buffer->open(QIODevice::ReadOnly);

	int sampleRate = source->GetSampleRate();
	int sampleSize = source->GetSampleSize();
	int channelsCount = source->GetChannelsCount();

	if(format.sampleRate() != sampleRate || format.sampleSize() != sampleSize || format.channelCount() != channelsCount)
	{
		format.setSampleRate(sampleRate);
		format.setSampleSize(sampleSize);
		format.setChannelCount(channelsCount);

		QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
		if(!info.isFormatSupported(format))
		{
			LUNA_LOGE("Audio format with sample rate:\"%d\", sample size:\"%d\" and channels count:\"%d\" not supported",
				sampleRate, sampleSize, channelsCount);
			return;
		}

		output = std::make_shared<QAudioOutput>(format, QApplication::instance());
		connect(output.get(), SIGNAL(stateChanged(QAudio::State)), this, SLOT(OnStateChanged(QAudio::State)));
	}

	output->start(buffer.get());
	//output->suspend();

	used = true;
}

void LUNAQtAudioPlayer::SetLoop(bool loop)
{

}

void LUNAQtAudioPlayer::Play()
{
	//output->resume();
}

void LUNAQtAudioPlayer::Pause()
{
	output->suspend();
}

void LUNAQtAudioPlayer::Stop()
{

}

void LUNAQtAudioPlayer::SetVolume(float volume)
{
	output->setVolume(volume);
}

void LUNAQtAudioPlayer::SetMute(bool mute)
{

}


LUNAQtAudio::LUNAQtAudio()
{
	for(int i = 0; i < AUDIO_PLAYERS_COUNT_QT; i++) players.push_back(std::make_shared<LUNAQtAudioPlayer>());
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
	auto it = buffers.find(bufferId);
	if(it == buffers.end()) return;

	// Stop players with given buffer id
	for(auto& player : players)
	{
		if(player->GetBufferId() == bufferId) player->Stop();
	}

	buffers.erase(bufferId);
}
