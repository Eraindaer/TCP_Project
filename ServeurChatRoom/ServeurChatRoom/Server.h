#pragma once
#include "WinSockManager.h"

class Server
{
public:
	Server() = default;
	Server(const WinSockManager& WSM);
	Server(const Server&);
	Server(Server&&) = default;
	~Server();
	Server& operator=(const Server&);
	Server& operator=(Server&&) = default;

	void InitSocket(const int& port);
	void ServerRoutine();

	void Close();
private:
	WinSockManager				WSM;
	SOCKET						server;
	fd_set						master;
};