#pragma once
#include "CommandManager.h"
#include "ErrorHandler.h"

class ChatRoom
{
public:
	ChatRoom(const WinSockManager& WSM);
	~ChatRoom();

	void InitServerConnection(const int& port);
	void InitSocket(const int& port, bool& isOpen);
	void RoutineChatRoom();

private:
	CommandManager		commandManager;
	WinSockManager		WSM;
	ErrorHandler        errorHandler;
	std::string			name;
	SOCKET				server; 
	fd_set				master;
	Logger              logger;
	int                 serverPort;
};