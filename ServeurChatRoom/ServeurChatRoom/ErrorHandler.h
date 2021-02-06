#pragma once
#include "WinSockManager.h"
#include "Logger.h"

template<typename T>
void TryCatchCheck(T f) {
	try {
		f();
	}
	catch (int& e) {
		switch (e) {
		case 16:
			std::cerr << "ERREUR : Impossible d'initialiser la session.\nFin de la session.\nCode d'erreur : " << e << std::endl;
			ExitProcess(EXIT_FAILURE);
			break;
		case 17:
			std::cerr << "ERREUR : Impossible de se connecter.\nFin de la session.\nCode d'erreur : " << e << std::endl;
			ExitProcess(EXIT_FAILURE);
			break;
		case 21:
			std::cerr << "ERREUR : Nom d�j� pris.\nVeuillez r�inscrire votre nom ainsi que le port.\nCode d'erreur : " << e << std::endl;
			break;
		case 22:
			std::cerr << "ERREUR : Port d�j� utilis� par le serveur g�n�ral.\nVeuillez en r�inscrire un autre.\nCode d'erreur : " << e << std::endl;
			break;
		default:
			std::cerr << "ERREUR : Erreur inconnue. Fin de la session.\nCode d'erreur : " << e << std::endl;
			ExitProcess(EXIT_FAILURE);
			break;
		}
	}
}

class ErrorHandler
{
public:
	ErrorHandler() = default;

	void HandleExceptionClient(WinSockManager& WSM, const std::exception& e, const SOCKET& sock, Logger& log) const;
	void HandleExceptionChatRoom(const std::exception& e, Logger& log) const;
};

