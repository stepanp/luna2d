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

#include "lunaqtaudio.h"
#include <QThread>
#include <QAudioOutput>

namespace luna2d{

class LUNAQtAudioThread : public QThread
{
	Q_OBJECT

public:
	LUNAQtAudioThread(LUNAQtAudio* audio);

private:
	LUNAQtAudio* audio;
	std::queue<LUNAQtAudioPlayer*> wrappers;
	QObject* playersHolder;

protected:
	virtual void run();

signals:
	void playersStopped(size_t bufferId);

public slots:
	// Stop all players with given id
	// After stopping players, emit "playersStopped" signal
	void StopPlayers(size_t bufferId);

public:
	void AttachPlayer(LUNAQtAudioPlayer* player);
};


class LUNAQtThreadPlayer : public QObject
{
	Q_OBJECT

public:
	LUNAQtThreadPlayer(QObject* parent);
	~LUNAQtThreadPlayer();

private:
	std::unique_ptr<QBuffer> buffer;
	std::unique_ptr<QAudioOutput> output;
	QAudioFormat format;
	bool inUse = false;

signals:
	void usingChanged(bool inUse);
	void playerStopped();

public slots:
	void OnStateChanged(QAudio::State state);
	void OnSetSource(QByteArray* bufferData, int sampleRate, int sampleSize, int channelsCount);
	void OnSetLoop(bool loop);
	void OnPlay();
	void OnPause();
	void OnStop();
	void OnRewind();
	void OnSetVolume(float volume);
	void OnSetMute(bool mute);

private:
	void InitAudioOutput(int sampleRate, int sampleSize, int channelsCount);
	void SetUsing(bool inUse);
};

}
