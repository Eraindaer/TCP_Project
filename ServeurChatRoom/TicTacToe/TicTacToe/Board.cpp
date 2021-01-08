#include "Board.h"

Board::Board(WindowManager* window) {
	bool random = true;
	this->window = window;
	
	player = new Player(this->window, random);
	computer = new Computer(this->window, !random);
	
	x = y = -1;
	playerTurn = random;

	this->window->GetTextureManager()->InitTexture("assets/board.png", this->window->GetRenderer(), tex);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			board[i][j] = 0;
}

Board::~Board() {
	delete(player);
	delete(computer);
	delete(window);
	SDL_DestroyTexture(tex);
}

void Board::Update(InputManager* inputManager){
	if (playerTurn) {
		if (player->Selection(inputManager)) {
			if (board[inputManager->GetMouseXPos()][inputManager->GetMouseYPos()] == 0) {
				std::cout << "Player turn \n";
				board[inputManager->GetMouseXPos()][inputManager->GetMouseYPos()] = 1;
				playerTurn = false;
			}
		}
	}
	else {
		std::cout << "Computer turn \n";
		computer->Update(x, y, board);
		board[x][y] = 2;
		playerTurn = true;
	}
	
}

void Board::Draw() {
	window->GetTextureManager()->DrawTexture(window->GetRenderer(), tex, { 0, 0, 500, 500 }, { 0, 0, 500, 500 });
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			switch (board[i][j])
			{
			case 1:
				player->Draw({i * window->GetWindowWidth() / 3 + 5, j * window->GetWindowHeight() / 3 + 5,160,160 });
				break;
			case 2:
				computer->Draw({ i * window->GetWindowWidth() / 3 + 5, j * window->GetWindowHeight() / 3 + 5, 160, 160 });
				break;
			default:
				break;
			}
		}
	}
}