#pragma once
#include "WinSockManager.h"

class Server
{
public:
	Server();
	~Server();
	Server(const Server&);
	Server& operator=(const Server&);
	Server(Server&&) = delete;
	Server& operator=(Server&&) = delete;

	void InitSocket(const int& port);
	void ServerRoutine() const;
	void Close() const;

private:
	std::unique_ptr<WinSockManager>				WSM = std::make_unique<WinSockManager>();
	SOCKET										server;
	fd_set										master;
};