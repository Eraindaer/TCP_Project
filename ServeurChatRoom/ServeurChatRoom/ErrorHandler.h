#pragma once
#include "WinSockManager.h"
#include "Logger.h"
class ErrorHandler
{
public:
	ErrorHandler();
	~ErrorHandler();

	void HandleExceptionClient(WinSockManager& WSM, const std::exception& e, const SOCKET& sock, Logger& log);
	void HandleExceptionChatRoom(const std::exception& e, Logger& log);
};

