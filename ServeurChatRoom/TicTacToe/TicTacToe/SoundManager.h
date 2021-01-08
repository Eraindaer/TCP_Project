#pragma once
#include "SDL_mixer.h"
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void InitMusic(const char* path, Mix_Music* &music);
	void InitSoundEffect(const char* path, Mix_Chunk* &effect);
	void PlayMusic(Mix_Music* music);
	void PlayEffect(Mix_Chunk* effect);
};