#include "MinMax.h"

Node::Node(){
	finishingNode = false;
	value = 0;
	coordonates[0] = coordonates[1] = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			board[i][j] = 0;
}

Node::~Node() {
	children.clear();
}

void Node::UpdateBoard(int board[3][3]) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			this->board[i][j] = board[i][j];
}

bool Node::CheckVictory() {
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == 2 && board[i][1] == 2 && board[i][2] == 2)
			return true;
		else if (board[0][i] == 2 && board[1][i] == 2 && board[2][i] == 2)
			return true;
	}
	if (board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2)
		return true;
	if (board[0][2] == 2 && board[1][1] == 2 && board[2][0] == 2)
		return true;
	return false;
}

bool Node::CheckDefeat() {
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == 1 && board[i][1] == 1 && board[i][2] == 1)
			return true;
		else if (board[0][i] == 1 && board[1][i] == 1 && board[2][i] == 1)
			return true;
	}
	if (board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1)
		return true;
	if (board[0][2] == 1 && board[1][1] == 1 && board[2][0] == 1)
		return true;
	return false;
}


NodeTree::NodeTree() {}

NodeTree::~NodeTree() {}

void NodeTree::Init(Node& currentNode, bool isComputer) {
	if (!currentNode.finishingNode) {
		if (isComputer) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (currentNode.board[i][j] == 0) {
						Node n;
						n.UpdateBoard(currentNode.board);
						n.board[i][j] = 2;
						n.coordonates[0] = i, n.coordonates[1] = j;
						if (n.CheckVictory() || n.CheckDefeat()) {

							n.finishingNode = true;
						}
						currentNode.children.push_back(n);
					}
				}
			}
			if (currentNode.children.size() != 0) {
				for (auto& child : currentNode.children) {
					Init(child, false);
				}
			}
			else {
				currentNode.finishingNode = true;
			}
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (currentNode.board[i][j] == 0) {
						Node n;
						n.UpdateBoard(currentNode.board);
						n.board[i][j] = 1;
						if (n.CheckVictory() || n.CheckDefeat()) {
							n.finishingNode = true;
						}
						currentNode.children.push_back(n);
					}
				}
			}
			if (currentNode.children.size() != 0) {
				for (auto& child : currentNode.children) {
					Init(child, true);
				}
			}
			else {
				currentNode.finishingNode = true;
			}
		}
	}
	else {
		if (currentNode.CheckVictory()) {
			currentNode.value = 10000;
		}
		else if (currentNode.CheckDefeat()) {
			currentNode.value = -10000;
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					switch (currentNode.board[i][j]) {
					case 1:
						currentNode.value--;
						break;
					case 2:
						currentNode.value++;
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

int NodeTree::MinMax(Node& currentNode, bool maximizingPlayer) {
	if (currentNode.finishingNode)
		return currentNode.value;
	if (maximizingPlayer) {
		currentNode.value = -HIGHNUMBER;
		for (auto& node : currentNode.children) {
			int eval = MinMax(node, false);
			currentNode.value = std::max(currentNode.value, eval);
		}
		return currentNode.value;
	}
	else {
		currentNode.value = HIGHNUMBER;
		for (auto& node : currentNode.children) {
			int eval = MinMax(node, true);
			currentNode.value = std::min(currentNode.value, eval);
		}
		return currentNode.value;
	}
}

void NodeTree::Clear(Node& currentNode) {
	if (!currentNode.finishingNode) {
		for (auto& child : currentNode.children) {
			Clear(child);
		}
	}
	currentNode.children.clear();
	currentNode.value = 0;
	currentNode.finishingNode = false;
	currentNode.coordonates[0] = currentNode.coordonates[1] = 0;
}