#include "SoundManager.h"

SoundManager::SoundManager() {
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 8, 4096);
}

SoundManager::~SoundManager() {
	Mix_CloseAudio();
}

void SoundManager::InitMusic(const char* path, Mix_Music* &music) {
	music = Mix_LoadMUS(path);
}

void SoundManager::InitSoundEffect(const char* path, Mix_Chunk*& effect) {
	effect = Mix_LoadWAV(path);
}

void SoundManager::PlayMusic(Mix_Music* music) {
	Mix_PlayMusic(music, -1);
}

void SoundManager::PlayEffect(Mix_Chunk* effect) {
	Mix_PlayChannel(-1, effect, 0);
}