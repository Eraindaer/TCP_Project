#pragma once
#include <random>
#include "Player.h"
#include "Computer.h"
class Board
{
public:
	Board(WindowManager* window);
	~Board();

	void Update(InputManager* inputManager);
	void Draw();

private:
	WindowManager* window;
	Player* player;
	Computer* computer;
	SDL_Texture* tex;
	bool playerTurn;
	int board[3][3];
	int x, y;
};

