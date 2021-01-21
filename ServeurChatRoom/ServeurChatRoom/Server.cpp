#include "Server.h"

Server::Server(const WinSockManager& WSM)
{
	this->WSM = WSM;
	std::cout << "***LANCEMENT DU SERVEUR***" << std::endl;
	std::cout << "\nVeuillez entrer le port du server" << std::endl;
}

Server::~Server()
{
}

void Server::InitSocket(const int& port)
{
	roomPort = port;
	SOCKADDR_IN sin;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = ADDR_ANY;
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	std::cout << "\nVeuillez ouvrir autant de chatrooms que vous le souhaitez" << std::endl;

	bind(server, (sockaddr*)&sin, sizeof(sin));
	listen(server, SOMAXCONN);
	FD_ZERO(&master);
	FD_SET(server, &master);
}

void Server::ServerRoutine()
{
	fd_set copy = master;

	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

	for (int i = 0; i < socketCount; i++) {
		SOCKET s = copy.fd_array[i];
		if (s == server) {
			SOCKET client = accept(server, nullptr, nullptr);
			FD_SET(client, &master);
			std::cout << "Nouvelle chatroom créée" << std::endl;
			std::string clName;
			WSM.RecieveMsg(client, clName);
			std::cout << "Bonjour " << clName << std::endl;
		}
	}
}
