#pragma once
#include "WindowManager.h"
#include "MinMax.h"
class Computer
{
public:
	Computer(WindowManager* window, bool first);
	~Computer();

	void Update(int& x, int& y, int board[3][3]);
	void Draw(SDL_Rect dest);

private:
	SDL_Texture* tex;
	WindowManager* window;
	Node node;
	NodeTree* tree;
};

