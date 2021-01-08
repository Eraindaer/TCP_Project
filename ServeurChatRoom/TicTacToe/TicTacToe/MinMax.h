#pragma once
#include <vector>
#include <iostream>
#define HIGHNUMBER 1000000000
class Node {
public:
	Node();
	~Node();

	void UpdateBoard(int board[3][3]);

	bool CheckVictory();
	bool CheckDefeat();

public:
	int value;
	int coordonates[2];
	int board[3][3];
	bool finishingNode;
	std::vector<Node> children;
};

class NodeTree
{
public:
	NodeTree();
	~NodeTree();

	void Init(Node& currentNode, bool isComputer);
	int MinMax(Node& currentNode, bool maximazingPlayer);
	void Clear(Node& currentNode);
};

