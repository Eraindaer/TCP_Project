#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib, "ws2_32.lib")

class WinSockManager
{
public:
	WinSockManager();
	~WinSockManager();
	WinSockManager(const WinSockManager&);
	WinSockManager& operator=(const WinSockManager&);
	WinSockManager(WinSockManager&&) = delete;
	WinSockManager& operator=(WinSockManager&&) = delete;

	void			SendMsg(const SOCKET& sock, const std::string& msg) const;
	void			RecieveMsg(const SOCKET& sock, std::string& msg) const;
	void            Close() const;

private:
	std::string		Compression(std::string sentence, std::map<char, std::string>& code) const;
	std::string		Decompression(std::map<char, std::string> code, std::string encodedText) const;

private:
	WSAData		    wsa;
};