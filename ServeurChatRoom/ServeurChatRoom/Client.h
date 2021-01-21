#pragma once
#include "WinSockManager.h"
#include <string>
#include <thread>

class Client
{
public:
	Client(const WinSockManager& WSM);
	~Client();

	void InitSocket(const int& port, const std::string& name);

	void SendingClient();
	void ReceivingClient();


private:
	WinSockManager WSM;
	SOCKET sock;
	std::string name;
};