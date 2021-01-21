#pragma once
#include "WinSockManager.h"
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <map>

class ChatRoom
{
public:
	ChatRoom(const WinSockManager& WSM);
	~ChatRoom();

	void InitServerConnection(const int& port);
	void InitSocket(const int& port);
	void InitCommands();
	void RoutineChatRoom();

private:
	std::map <std::string, std::function<void(const std::vector<std::string>&, const SOCKET&)>> commandList;
	std::string name;
	WinSockManager WSM;
	SOCKET server; 
	fd_set master;
};