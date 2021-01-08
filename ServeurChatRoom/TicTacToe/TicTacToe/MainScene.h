#pragma once
#include "Board.h"
#include "SoundManager.h"
#define FPS 60
#define FRAMEDELAY (1000/FPS)
class MainScene
{
public:
	MainScene(WindowManager* window);
	~MainScene();

	void			Events();
	void			Update();
	void			Draw();
	void            SoundEffects();
	void			Delay();

	void			Clear();

private:
	Board*			board;
	Mix_Music*      music;
	Uint32			frameStart;
	int				frameTime;
	WindowManager*  window;
	SoundManager*   soundManager;
	TextureManager* textureManager;
	InputManager*	inputManager;
};

