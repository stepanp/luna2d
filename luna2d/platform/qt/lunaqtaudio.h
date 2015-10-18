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

#include "lunaaudio.h"
#include <QAudioOutput>
#include <QBuffer>

namespace luna2d{

//------------------------------------------
// Helper audio player implementation for Qt
//------------------------------------------
class LUNAQtAudioPlayer : public QObject, public LUNAAudioPlayer
{
	Q_OBJECT

private:
	std::shared_ptr<QAudioOutput> output;
	std::shared_ptr<QBuffer> buffer;

public:
	virtual void SetSource(const std::weak_ptr<LUNAAudioSource>& source);

	virtual void Play();

	virtual void Pause();

	virtual void Stop();

	virtual void SetVolume(float volume);

	virtual void SetMute(bool mute);
};


//----------------------------
// Audio implementation for Qt
//----------------------------
class LUNAQtAudio : public LUNAAudio
{
private:
	std::unordered_map<size_t, std::shared_ptr<QBuffer>> buffers;
	size_t uniqueBufferId;

public:
	// Get buffer by given buffer id
	virtual std::shared_ptr<QBuffer> GetBuffer(size_t bufferId);

	// Create audio buffer from given audio data
	// In case of success return id of created buffer, else return 0
	virtual size_t CreateBuffer(const std::vector<unsigned char>& data);

	// Release buffer with given id
	// All plyers using same buffer should be stopped
	virtual void ReleaseBuffer(size_t bufferId);

	// Play sound from given source
	virtual void PlaySound(const std::weak_ptr<LUNAAudioSource>& source);
};

}
