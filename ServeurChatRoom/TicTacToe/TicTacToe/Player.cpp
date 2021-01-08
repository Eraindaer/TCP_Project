#include "Player.h"

Player::Player(WindowManager* window, bool first) {
	this->window = window;
	if (first)
		this->window->GetTextureManager()->InitTexture("assets/cross.png", this->window->GetRenderer(), tex);
	else
		this->window->GetTextureManager()->InitTexture("assets/circle.png", this->window->GetRenderer(), tex);
}

Player::~Player() {
	delete(window);
	SDL_DestroyTexture(tex);
}

bool Player::Selection(InputManager* inputManager){
	if (inputManager->GetMouseClick())
		return true;
	return false;
}

void Player::Draw(SDL_Rect dest) {
	window->GetTextureManager()->DrawTexture(window->GetRenderer(), tex, { 0, 0, 39, 39 }, dest);
}