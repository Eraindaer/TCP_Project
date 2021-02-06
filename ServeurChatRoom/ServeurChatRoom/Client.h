#pragma once
#include "WinSockManager.h"

class Client
{
public:
	Client() = default;
	Client(const WinSockManager& WSM);
	Client(const Client&);
	Client(Client&&) = default;
	~Client();
	Client& operator=(const Client&);
	Client& operator=(Client&&) = default;

	void InitSocket(const int& port, const std::string& name, bool& isConnected);

	void SendingClient();
	void ReceivingClient();
	void Close() const;

private:
	WinSockManager		WSM;
	std::string			name;
	SOCKET				sock;
};