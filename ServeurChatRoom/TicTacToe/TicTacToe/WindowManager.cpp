#include "WindowManager.h"

WindowManager::WindowManager(const char* title, int xPos, int yPos, int width, int height, SDL_Color color) {
	event.type = NULL;
	isRunning = false;
	textureManager = nullptr;
	window = nullptr;
	windowRenderer = NULL;
	this->width = width, this->height = height;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, xPos, yPos, width, height, 0);
		windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(windowRenderer, color.r, color.g, color.b, color.a);
		isRunning = true;
		textureManager = new TextureManager();
	}
}
WindowManager::~WindowManager() {
	delete(textureManager);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(windowRenderer);
}

void WindowManager::FrameEvents() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		break;
	default:
		break;
	}
}