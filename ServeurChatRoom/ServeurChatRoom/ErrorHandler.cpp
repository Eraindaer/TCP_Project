#include "ErrorHandler.h"

void ErrorHandler::HandleExceptionClient(WinSockManager& WSM, const std::exception& e, const SOCKET& sock, Logger& log) const
{
	WSM.SendMsg(sock, e.what());
	log.WriteError("ERREUR : ", e);
}

void ErrorHandler::HandleExceptionChatRoom(const std::exception& e, Logger& log) const
{
	std::cout << "ERREUR : " << e.what() << std::endl;
	log.WriteError("ERREUR : ", e);
}
