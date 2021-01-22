#pragma once
#include "WinSockManager.h"

class Client
{
public:
	Client(const WinSockManager& WSM);
	~Client();

	void InitSocket(const int& port, const std::string& name, bool& isConnected);

	void SendingClient();
	void ReceivingClient();


private:
	WinSockManager		WSM;
	std::string			name;
	SOCKET				sock;
};