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

#include "lunaaudio.h"

using namespace luna2d;

size_t LUNAAudioPlayer::GetBufferId()
{
	return bufferId;
}


std::shared_ptr<LUNAAudioPlayer> LUNAAudio::FindFreePlayer(const std::shared_ptr<LUNAAudioSource>& source)
{
	auto it = std::find_if(players.begin(), players.end(),
		[](const std::shared_ptr<LUNAAudioPlayer>& player) { return !player->IsUsing(); });
	if(it == players.end()) return nullptr;
	return *it;
}

// Play background music from given audio source
void LUNAAudio::PlayMusic(const std::weak_ptr<LUNAAudioSource>& source)
{
	if(source.expired()) LUNA_RETURN_ERR("Attempt to play invalid audio source");

	musicPlayer->Stop();
	musicPlayer->SetSource(source.lock());
	musicPlayer->Play();
}

// Play sound from given source
void LUNAAudio::PlaySound(const std::weak_ptr<LUNAAudioSource>& source)
{
	if(source.expired()) LUNA_RETURN_ERR("Attempt to play invalid audio source");

	auto sharedSource = source.lock();

	auto player = FindFreePlayer(sharedSource);
	if(!player) LUNA_RETURN_ERR("Cannot play audio source. All audio players are used");

	player->SetSource(sharedSource);
	player->Play();
}

// Stop all currently playing sounds
void LUNAAudio::StopAllSounds()
{
	for(auto& player : players) player->Stop();
}
