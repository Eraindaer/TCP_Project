#pragma once
#include "WinSockManager.h"

class Server
{
public:
	Server(const WinSockManager& WSM);
	~Server();

	void InitSocket(const int& port);
	void ServerRoutine();

private:
	WinSockManager				WSM;
	SOCKET						server;
	fd_set						master;
};