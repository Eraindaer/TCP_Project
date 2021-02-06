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

	void			SendMsg(const SOCKET& sock, const std::string& msg);
	void			RecieveMsg(const SOCKET& sock, std::string& msg);

	void            Close() const;

	std::string		Compression(std::string sentence, std::map<char, std::string>& code);
	std::string		Decompression(std::map<char, std::string> code, std::string encodedText);
};