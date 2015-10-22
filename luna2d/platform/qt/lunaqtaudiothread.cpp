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

#include "lunaqtaudiothread.h"

using namespace luna2d;

LUNAQtAudioThread::LUNAQtAudioThread(LUNAQtAudio* audio) :
	audio(audio)
{
}

void LUNAQtAudioThread::run()
{
	QObject scopedHolder;
	playersHolder = &scopedHolder;

	while(!wrappers.empty())
	{
		LUNAQtThreadPlayer* player = new LUNAQtThreadPlayer(&scopedHolder);
		LUNAQtAudioPlayer* wrapper = wrappers.front();
		wrappers.pop();

		connect(player, &LUNAQtThreadPlayer::usingChanged, wrapper, &LUNAQtAudioPlayer::OnUsingChanged);
		connect(player, &LUNAQtThreadPlayer::playerStopped, wrapper, &LUNAQtAudioPlayer::OnPlayerStopped);

		connect(wrapper, &LUNAQtAudioPlayer::setSource, player, &LUNAQtThreadPlayer::OnSetSource);
		connect(wrapper, &LUNAQtAudioPlayer::setLoop, player, &LUNAQtThreadPlayer::OnSetLoop);
		connect(wrapper, &LUNAQtAudioPlayer::play, player, &LUNAQtThreadPlayer::OnPlay);
		connect(wrapper, &LUNAQtAudioPlayer::pause, player, &LUNAQtThreadPlayer::OnPause);
		connect(wrapper, &LUNAQtAudioPlayer::stop, player, &LUNAQtThreadPlayer::OnStop);
		connect(wrapper, &LUNAQtAudioPlayer::rewind, player, &LUNAQtThreadPlayer::OnRewind);
		connect(wrapper, &LUNAQtAudioPlayer::setVolume, player, &LUNAQtThreadPlayer::OnSetVolume);
		connect(wrapper, &LUNAQtAudioPlayer::setMute, player, &LUNAQtThreadPlayer::OnSetMute);
	}

	connect(audio, &LUNAQtAudio::stopPlayers, this, &LUNAQtAudioThread::StopPlayers);
	connect(this, &LUNAQtAudioThread::playersStopped, audio, &LUNAQtAudio::OnPlayersStopped);

	exec();
}

// Stop all players with given id
// After stopping players, emit "playersStopped" signal
void LUNAQtAudioThread::StopPlayers(size_t bufferId)
{
	auto players = playersHolder->children();
	for(QObject* player : players)
	{
		(static_cast<LUNAQtThreadPlayer*>(player))->OnStop();
	}

	emit playersStopped(bufferId);
}

void LUNAQtAudioThread::AttachPlayer(LUNAQtAudioPlayer* player)
{
	wrappers.push(player);
}


LUNAQtThreadPlayer::LUNAQtThreadPlayer(QObject* parent) :
	QObject(parent),
	buffer(std::unique_ptr<QBuffer>(new QBuffer()))
{
	InitAudioOutput(DEFAULT_SAMPLE_RATE, DEFAULT_SAMPLE_SIZE, DEFAULT_CHANNELS_COUNT);
}

LUNAQtThreadPlayer::~LUNAQtThreadPlayer()
{
	OnStop();
}

void LUNAQtThreadPlayer::OnStateChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::IdleState:
		OnStop();
	}
}

void LUNAQtThreadPlayer::OnSetSource(QByteArray* bufferData, int sampleRate, int sampleSize, int channelsCount)
{
	buffer->setBuffer(bufferData);
	buffer->open(QIODevice::ReadOnly);

	if(format.sampleRate() != sampleRate || format.sampleSize() != sampleSize || format.channelCount() != channelsCount)
	{
		InitAudioOutput(sampleRate, sampleSize, channelsCount);
	}

	SetUsing(output != nullptr);
}

void LUNAQtThreadPlayer::OnSetLoop(bool loop)
{

}

void LUNAQtThreadPlayer::OnPlay()
{
	if(!output || !inUse) return;

	if(output->state() == QAudio::SuspendedState) output->resume();
	else output->start(buffer.get());
}

void LUNAQtThreadPlayer::OnPause()
{
	if(!output || !inUse) return;

	output->suspend();
}

void LUNAQtThreadPlayer::OnStop()
{
	if(!output || !inUse) return;

	output->stop();
	buffer->close();
	SetUsing(false);

	emit playerStopped();
}

void LUNAQtThreadPlayer::OnRewind()
{
	if(!output || !inUse) return;

	buffer->seek(0);
}

void LUNAQtThreadPlayer::OnSetVolume(float volume)
{
	if(!output || !inUse) return;

	output->setVolume(volume);
}

void LUNAQtThreadPlayer::OnSetMute(bool mute)
{

}

void LUNAQtThreadPlayer::InitAudioOutput(int sampleRate, int sampleSize, int channelsCount)
{
	format.setSampleRate(sampleRate);
	format.setSampleSize(sampleSize);
	format.setChannelCount(channelsCount);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(sampleSize == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if(!info.isFormatSupported(format) && LUNAEngine::SharedLog())
	{
		LUNA_LOGE("Audio format with sample rate:\"%d\", sample size:\"%d\" and channels count:\"%d\" not supported",
			sampleRate, sampleSize, channelsCount);
		output = nullptr;
		return;
	}

	output = std::unique_ptr<QAudioOutput>(new QAudioOutput(format, this));
	connect(output.get(), SIGNAL(stateChanged(QAudio::State)), this, SLOT(OnStateChanged(QAudio::State)));
}

void LUNAQtThreadPlayer::SetUsing(bool inUse)
{
	this->inUse = inUse;
	emit usingChanged(inUse);
}
