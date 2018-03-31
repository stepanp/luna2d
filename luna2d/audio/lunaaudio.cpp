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

#include "lunaaudio.h"

using namespace luna2d;

LUNAAudioPlayer::LUNAAudioPlayer()
{
	alGenSources(1, &soundId);
}

LUNAAudioPlayer::~LUNAAudioPlayer()
{
	Stop();
	if(alIsSource(soundId)) alDeleteSources(1, &soundId);
}

ALuint LUNAAudioPlayer::GetSourceId()
{
	return soundId;
}

bool LUNAAudioPlayer::IsUsing()
{
	if(isUsing)
	{
		ALint state;
		alGetSourcei(soundId, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING || state == AL_PAUSED;
	}

	return isUsing;
}

bool LUNAAudioPlayer::IsPlaying()
{
	if(isUsing)
	{
		ALint state;
		alGetSourcei(soundId, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	return false;
}

void LUNAAudioPlayer::SetSource(ALuint sourceId)
{
	this->sourceId = sourceId;
	isUsing = true;

	alSourcei(soundId, AL_BUFFER, sourceId);
}

void LUNAAudioPlayer::SetLoop(bool loop)
{
	 alSourcei(soundId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void LUNAAudioPlayer::Play()
{
	alSourcePlay(soundId);
}

void LUNAAudioPlayer::Pause()
{
	alSourcePause(soundId);
}

void LUNAAudioPlayer::Stop()
{
	alSourceStop(soundId);
	isUsing = false;
}

void LUNAAudioPlayer::Rewind()
{
	alSourceRewind(soundId);
}

void LUNAAudioPlayer::SetVolume(float volume)
{
	alSourcef(soundId, AL_GAIN, volume);
}

void LUNAAudioPlayer::SetMute(bool mute)
{

}

void LUNAAudioPlayer::OnPause()
{
	if(IsPlaying())
	{
		Pause();
		backgroundPause = true;
	}
}

void LUNAAudioPlayer::OnResume()
{
	if(backgroundPause)
	{
		Play();
		backgroundPause = false;
	}
}


LUNAAudio::LUNAAudio()
{
	device = alcOpenDevice(nullptr);
	if(!device) LUNA_RETURN_ERR("Cannot open OpenAL audio device");

	context = alcCreateContext(device, nullptr);
	if(!context) LUNA_RETURN_ERR("Cannot open OpenAL audio context");

	alcMakeContextCurrent(context);

	for(int i = 0; i < SOUND_PLAYERS_COUNT; i++) players.push_back(std::make_shared<LUNAAudioPlayer>());

	musicPlayer = std::make_shared<LUNAAudioPlayer>();
	musicPlayer->SetLoop(true);
}

LUNAAudio::~LUNAAudio()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

int LUNAAudio::FindFreePlayerIndex()
{
	auto it = std::find_if(players.begin(), players.end(),
		[](const std::shared_ptr<LUNAAudioPlayer>& player) { return !player->IsUsing(); });
	if(it == players.end()) return -1;
	return it - players.begin();
}

// Check is music playing
bool LUNAAudio::IsMusicPlaying()
{
	if(!context) return false;

	return musicPlayer->IsUsing();
}

// Play background music from given audio source
void LUNAAudio::PlayMusic(const std::weak_ptr<LUNAAudioSource>& source)
{
	if(!context) return;
	if(source.expired()) LUNA_RETURN_ERR("Attempt to play invalid audio source");

	musicPlayer->Stop();
	musicPlayer->SetSource(source.lock()->GetId());
	musicPlayer->Play();
}

// Stop background music
void LUNAAudio::StopMusic()
{
	if(!context) return;

	musicPlayer->Stop();
}

// Play sound from given source
int LUNAAudio::PlaySound(const std::weak_ptr<LUNAAudioSource>& source, float volume)
{
	if(!context) return -1;
	if(source.expired())
	{
		LUNA_LOGE("Attempt to play invalid audio source");
		return -1;
	}
	if(volume < 0.0f && volume > 1.0f)
	{
		LUNA_LOGE("Volume should be in range [0.0f, 1.0f]");
		return -1;
	}

	int playerIndex = FindFreePlayerIndex();
	if(playerIndex == -1)
	{
		LUNA_LOGE("Cannot play audio source. All audio players are used");
		return -1;
	}

	auto player = players[playerIndex];
	float playerVolume = muteSound ? 0.0f : (soundVolume * volume);

	player->SetSource(source.lock()->GetId());
	player->SetVolume(playerVolume);
	player->Play();

	return playerIndex;
}

// Play looped sound from given audio source
int LUNAAudio::PlayLoop(const std::weak_ptr<LUNAAudioSource>& source, float volume)
{
	int playerIndex = PlaySound(source, volume);
	if(playerIndex == -1) return -1;

	players[playerIndex]->SetLoop(true);

	return playerIndex;
}

// Stop sound by player index
void LUNAAudio::StopSound(int playerIndex)
{
	if(playerIndex < 0 || playerIndex >= players.size()) LUNA_RETURN_ERR("Ivalid sound id");

	auto player = players[playerIndex];
	player->Stop();
	player->SetLoop(false);
}

// Stop all currently playing sounds
void LUNAAudio::StopAllSounds()
{
	if(!context) return;

	for(auto& player : players)
	{
		player->Stop();
		player->SetLoop(false);
	}
}

// Get master volume for music
float LUNAAudio::GetMusicVolume()
{
	return musicVolume;
}

// Set master volume for music
// "volume" should be in range [0.0f, 1.0f]
void LUNAAudio::SetMusicVolume(float volume)
{
	if(!context) return;
	if(volume < 0.0f && volume > 1.0f) LUNA_RETURN_ERR("Volume should be in range [0.0f, 1.0f]");

	musicVolume = volume;
	musicPlayer->SetVolume(volume);
}

// Get master volume for sounds
float LUNAAudio::GetSoundVolume()
{
	return soundVolume;
}

// Set master volume for sounds
// "volume" should be in range [0.0f, 1.0f]
void LUNAAudio::SetSoundVolume(float volume)
{
	if(!context) return;
	if(volume < 0.0f && volume > 1.0f) LUNA_RETURN_ERR("Volume should be in range [0.0f, 1.0f]");

	soundVolume = volume;
	for(auto& player : players) player->SetVolume(volume);
}

// Stop all players with given source id
void LUNAAudio::StopPlayersWithSource(ALuint sourceId)
{
	if(!context) return;

	if(musicPlayer->GetSourceId() == sourceId) musicPlayer->Stop();
	for(auto& player : players)
	{
		if(player->GetSourceId() == sourceId)
		{
			player->Stop();
			player->SetLoop(false);
		}
	}
}

// Check is music muted
bool LUNAAudio::IsMusicMuted()
{
	return muteMusic;
}

// Mute\unmute music
void LUNAAudio::MuteMusic(bool mute)
{
	if(!context) return;

	muteMusic = mute;
	musicPlayer->SetVolume(mute ? 0.0f : musicVolume);
}

// Check is sound muted
bool LUNAAudio::IsSoundMuted()
{
	return muteSound;
}

// Mute\unmute all sounds
void LUNAAudio::MuteSound(bool mute)
{
	if(!context) return;

	muteSound = mute;
	for(auto& player : players) player->SetVolume(mute ? 0.0f : soundVolume);
}

// Pause audio when engine is pausing
void LUNAAudio::OnPause()
{
	if(!context) return;

	musicPlayer->OnPause();
	for(auto& player : players) player->OnPause();

#ifdef ALC_SOFT_pause_device
	alcDevicePauseSOFT(device);
#endif
}

// Resume audio when engine is resuming
void LUNAAudio::OnResume()
{
	if(!context) return;

#ifdef ALC_SOFT_pause_device
	alcDeviceResumeSOFT(device);
#endif

	musicPlayer->OnResume();
	for(auto& player : players) player->OnResume();
}
