#pragma once
#include "CommandManager.h"
#include "ErrorHandler.h"

class ChatRoom
{
public:
	ChatRoom() = default;
	ChatRoom(const WinSockManager& WSM);
	ChatRoom(const ChatRoom&);
	ChatRoom(ChatRoom&&) = default;
	~ChatRoom();
	ChatRoom& operator=(const ChatRoom&);
	ChatRoom& operator=(ChatRoom&&) = default;

	void InitServerConnection(const int& port);
	void InitSocket(const int& port, bool& isOpen);
	void RoutineChatRoom();

	void Close();

private:
	std::unique_ptr<CommandManager>		commandManager = std::make_unique<CommandManager>(); 
	std::unique_ptr<Logger>             logger = std::make_unique<Logger>();
	WinSockManager						WSM;
	ErrorHandler						errorHandler;
	std::string							name;
	SOCKET								server; 
	fd_set								master;
	int									serverPort;
};