#pragma once
#include "WindowManager.h"
#include "InputManager.h"
class Player
{
public:
	Player(WindowManager* window, bool first);
	~Player();

	bool Selection(InputManager* inputManager);
	void Draw(SDL_Rect dest);

private:
	SDL_Texture* tex;
	WindowManager* window;	
};

