#pragma once
#include "WinSockManager.h"

class Client
{
public:
	Client();
	~Client();
	Client(const Client&);
	Client& operator=(const Client&);
	Client(Client&&) = delete;
	Client& operator=(Client&&) = delete;

	void InitSocket(const int& port, const std::string& name, bool& isConnected);
	void SendingClient() const;
	void ReceivingClient() const;
	void Close() const;

private:
	std::unique_ptr<WinSockManager>		WSM = std::make_unique<WinSockManager>();
	std::string							name;
	SOCKET								sock;
};