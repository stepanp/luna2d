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

#include "lunaqtaudioworker.h"

using namespace luna2d;

// Create worker player in audio thread
// and connect it to given audio player interface
void LUNAQtWorkerManager::RequestWorker(LUNAQtAudioPlayer* player)
{
	// This manager is parent for worker
	// So, worker will be safety deleted with deleting manager
	LUNAQtWorkerPlayer* worker = new LUNAQtWorkerPlayer(this);

	connect(worker, &LUNAQtWorkerPlayer::usingChanged, player, &LUNAQtAudioPlayer::OnUsingChanged);

	connect(player, &LUNAQtAudioPlayer::workerSetSource, worker, &LUNAQtWorkerPlayer::SetSource);
	connect(player, &LUNAQtAudioPlayer::workerSetLoop, worker, &LUNAQtWorkerPlayer::SetLoop);
	connect(player, &LUNAQtAudioPlayer::workerPlay, worker, &LUNAQtWorkerPlayer::Play);
	connect(player, &LUNAQtAudioPlayer::workerPause, worker, &LUNAQtWorkerPlayer::Pause);
	connect(player, &LUNAQtAudioPlayer::workerStop, worker, &LUNAQtWorkerPlayer::Stop);
	connect(player, &LUNAQtAudioPlayer::workerRewind, worker, &LUNAQtWorkerPlayer::Rewind);
	connect(player, &LUNAQtAudioPlayer::workerSetVolume, worker, &LUNAQtWorkerPlayer::SetVolume);
	connect(player, &LUNAQtAudioPlayer::workerSetMute, worker, &LUNAQtWorkerPlayer::SetMute);
}

// Stop all workers using buffer with given id
// After stopping workers, emit "workersStopped" signal
void LUNAQtWorkerManager::StopWorkers(size_t bufferId)
{
	QThread* thread = QThread::currentThread();

	auto workers = this->children();
	for(QObject* child : workers)
	{
		LUNAQtWorkerPlayer* worker = qobject_cast<LUNAQtWorkerPlayer*>(child);
		if(worker->IsUsing() && worker->GetBufferId() == bufferId) worker->Stop();
	}

	emit workersStopped(bufferId);
}


LUNAQtWorkerPlayer::LUNAQtWorkerPlayer(QObject* parent) :
	QObject(parent),
	buffer(std::unique_ptr<QBuffer>(new QBuffer()))
{
	InitAudioOutput(DEFAULT_SAMPLE_RATE, DEFAULT_SAMPLE_SIZE, DEFAULT_CHANNELS_COUNT);
}

LUNAQtWorkerPlayer::~LUNAQtWorkerPlayer()
{
	Stop();
}

void LUNAQtWorkerPlayer::OnStateChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::IdleState:
		if(loop)
		{
			Rewind();
			Play();
		}
		else Stop();
		break;
	}
}

void LUNAQtWorkerPlayer::SetSource(size_t bufferId, QByteArray* bufferData, int sampleRate, int sampleSize, int channelsCount)
{
	this->bufferId = bufferId;

	buffer->setBuffer(bufferData);
	buffer->open(QIODevice::ReadOnly);

	if(format.sampleRate() != sampleRate || format.sampleSize() != sampleSize || format.channelCount() != channelsCount)
	{
		InitAudioOutput(sampleRate, sampleSize, channelsCount);
	}

	SetUsing(output != nullptr);
}

void LUNAQtWorkerPlayer::SetLoop(bool loop)
{
	this->loop = loop;
}

void LUNAQtWorkerPlayer::Play()
{
	if(!output || !inUse) return;

	if(output->state() == QAudio::SuspendedState) output->resume();
	else output->start(buffer.get());
}

void LUNAQtWorkerPlayer::Pause()
{
	if(!output || !inUse) return;

	output->suspend();
}

void LUNAQtWorkerPlayer::Stop()
{
	if(!output || !inUse) return;

	output->stop();
	buffer->close();

	SetUsing(false);
}

void LUNAQtWorkerPlayer::Rewind()
{
	if(!output || !inUse) return;

	buffer->seek(0);
}

void LUNAQtWorkerPlayer::SetVolume(float volume)
{
	if(!output || !inUse) return;

	output->setVolume(volume);
}

void LUNAQtWorkerPlayer::SetMute(bool mute)
{

}

void LUNAQtWorkerPlayer::InitAudioOutput(int sampleRate, int sampleSize, int channelsCount)
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

void LUNAQtWorkerPlayer::SetUsing(bool inUse)
{
	this->inUse = inUse;
	emit usingChanged(inUse);
}

size_t LUNAQtWorkerPlayer::GetBufferId()
{
	return bufferId;
}

bool LUNAQtWorkerPlayer::IsUsing()
{
	return inUse;
}
