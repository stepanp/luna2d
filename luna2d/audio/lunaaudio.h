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

#pragma once

#include "lunaengine.h"
#include "lunaaudiosource.h"

namespace luna2d{

const int SOUND_PLAYERS_COUNT = 15;

class LUNAAudioPlayer
{
public:
	LUNAAudioPlayer();
	~LUNAAudioPlayer();

protected:
	ALuint soundId;
	ALuint sourceId;
	bool isUsing = false;
	bool backgroundPause = false;

public:
	ALuint GetSourceId();
	bool IsUsing();
	bool IsPlaying();
	void SetSource(ALuint sourceId);
	void SetLoop(bool loop);
	void Play();
	void Pause();
	void Stop();
	void Rewind();
	void SetVolume(float volume);
	void SetMute(bool mute);
	void OnPause();
	void OnResume();
};


class LUNAAudio
{
public:
	LUNAAudio();
	~LUNAAudio();

private:
	ALCdevice* device;
	ALCcontext* context;
	std::vector<std::shared_ptr<LUNAAudioPlayer>> players;
	std::shared_ptr<LUNAAudioPlayer> musicPlayer;
	float musicVolume = 1.0f;
	float soundVolume = 1.0f;
	bool muteMusic = false;
	bool muteSound = false;

protected:
	std::shared_ptr<LUNAAudioPlayer> FindFreePlayer();

public:
	// Check is music playing
	bool IsMusicPlaying();

	// Play background music from given audio source
	void PlayMusic(const std::weak_ptr<LUNAAudioSource>& source);

	// Stop background music
	void StopMusic();

	// Play sound from given audio source
	void PlaySound(const std::weak_ptr<LUNAAudioSource>& source);

	// Stop all currently playing sounds
	void StopAllSounds();

	// Get master volume for music
	float GetMusicVolume();

	// Set master volume for music
	// "volume" should be in range [0.0f, 1.0f]
	void SetMusicVolume(float volume);

	// Get master volume for sounds
	float GetSoundVolume();

	// Set master volume for sounds
	// "volume" should be in range [0.0f, 1.0f]
	void SetSoundVolume(float volume);

	// Stop all players with given source id
	void StopPlayersWithSource(ALuint sourceId);

	// Check is music muted
	bool IsMusicMuted();

	// Mute\unmute music
	void MuteMusic(bool mute);

	// Check is sound muted
	bool IsSoundMuted();

	// Mute\unmute all sounds
	void MuteSound(bool mute);

	// Pause audio when engine is pausing
	void OnPause();

	// Resume audio when engine is resuming
	void OnResume();
};

}
