#include "MainScene.h"

MainScene::MainScene(WindowManager* window) {
	this->window = window;
	textureManager = window->GetTextureManager();
	frameStart = frameTime = 0;

	soundManager = new SoundManager();
	inputManager = new InputManager(window);
	board = new Board(this->window);

	soundManager->InitMusic("assets/", music);

	/*
		Initialiser les différents éléments de la scène
	*/
}

MainScene::~MainScene() {
	delete(soundManager);
}

void MainScene::Events() {
	frameStart = SDL_GetTicks();
	window->FrameEvents();
}

void MainScene::Update() {
	board->Update(inputManager);
}

void MainScene::Draw() {
	SDL_RenderClear(window->GetRenderer());
	board->Draw();
	SDL_RenderPresent(window->GetRenderer());
}

void MainScene::SoundEffects() {
	soundManager->PlayMusic(music);
}

void MainScene::Delay() {
	frameTime = SDL_GetTicks() - frameStart;
	if (frameTime < FRAMEDELAY) {
		SDL_Delay(FRAMEDELAY - frameTime);
	}
}

void MainScene::Clear() {
	SDL_Quit();
}