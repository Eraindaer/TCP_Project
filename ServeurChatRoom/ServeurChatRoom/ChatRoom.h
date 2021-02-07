#pragma once
#include "CommandManager.h"
#include "ErrorHandler.h"

class ChatRoom
{
public:
	ChatRoom();
	~ChatRoom();
	ChatRoom(const ChatRoom&);
	ChatRoom& operator=(const ChatRoom&);
	ChatRoom(ChatRoom&&) = delete;
	ChatRoom& operator=(ChatRoom&&) = delete;

	void InitServerConnection(const int& port);
	void InitSocket(const int& port, bool& isOpen);
	void RoutineChatRoom() const;
	void Close() const;

private:
	std::unique_ptr<CommandManager>		commandManager = std::make_unique<CommandManager>(); 
	std::unique_ptr<WinSockManager>		WSM = std::make_unique<WinSockManager>();
	std::unique_ptr<ErrorHandler>		errorHandler = std::make_unique<ErrorHandler>();
	std::unique_ptr<Logger>             logger = std::make_unique<Logger>();
	std::string							name;
	SOCKET								server; 
	fd_set								master;
	int									serverPort;
};