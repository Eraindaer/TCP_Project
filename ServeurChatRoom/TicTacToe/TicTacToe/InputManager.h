#pragma once
#include "WindowManager.h"
class InputManager
{
public:
	InputManager(WindowManager* window);
	~InputManager();

	bool GetMouseClick();
	int GetMouseXPos();
	int GetMouseYPos();

private:
	WindowManager* window;
};

