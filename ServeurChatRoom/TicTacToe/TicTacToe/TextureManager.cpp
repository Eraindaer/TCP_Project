#include "TextureManager.h"

TextureManager::TextureManager() {}

TextureManager::~TextureManager(){}

void TextureManager::InitTexture(const char* path, SDL_Renderer* renderer, SDL_Texture*& tex) {
	SDL_Surface* tempSurface = IMG_Load(path);
	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void TextureManager::DrawTexture(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(renderer, tex, &src, &dest);
}
