#include "Computer.h"

Computer::Computer(WindowManager* window, bool first) {
	this->window = window;
	if (first)
		this->window->GetTextureManager()->InitTexture("assets/cross.png", this->window->GetRenderer(), tex);
	else
		this->window->GetTextureManager()->InitTexture("assets/circle.png", this->window->GetRenderer(), tex);

	tree = new NodeTree();
}

Computer::~Computer() {
	delete(window);
	SDL_DestroyTexture(tex);
}
void Computer::Update(int& x, int& y, int board[3][3]) {
	node.UpdateBoard(board);
	tree->Init(node, true);
	int eval = tree->MinMax(node, true);
	for (auto& child : node.children) {
		if (child.value == eval) {
			x = child.coordonates[0], y = child.coordonates[1];
			std::cout << "Coordonates : " << x << ", " << y << "\n";
			break;
		}
	}
	tree->Clear(node);
}

void Computer::Draw(SDL_Rect dest) {
	window->GetTextureManager()->DrawTexture(window->GetRenderer(), tex, { 0, 0, 39, 39 }, dest);
}