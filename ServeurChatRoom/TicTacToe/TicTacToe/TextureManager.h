#pragma once
#include "SDL.h"
#include "SDL_image.h"
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void InitTexture(const char* path, SDL_Renderer* renderer, SDL_Texture* &tex);
	void DrawTexture(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};

