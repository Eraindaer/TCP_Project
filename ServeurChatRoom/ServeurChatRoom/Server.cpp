#include "Server.h"

Server::Server()
{
	server = NULL;
	FD_ZERO(&master);
	std::cout << "***LANCEMENT DU SERVEUR***" << std::endl;
	std::cout << "\nVeuillez entrer le port du server" << std::endl;
}

Server::~Server()
{
	Close();
	WSM->Close();
}

Server::Server(const Server& newServer)
{
	WSM = std::make_unique<WinSockManager>(*newServer.WSM);
	server = newServer.server;
	FD_ZERO(&master);
	master = newServer.master;
}

Server& Server::operator=(const Server& copyAssign)
{
	WSM = std::make_unique<WinSockManager>(*copyAssign.WSM);
	closesocket(server);
	server = copyAssign.server;
	FD_ZERO(&master);
	master = copyAssign.master;
	return *this;
}

void Server::InitSocket(const int& port) 
{
	SOCKADDR_IN sin;
	server = socket(AF_INET, SOCK_STREAM, 0);

	//Vérification du socket server
	if (server != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = ADDR_ANY;
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else {
		throw 16;
		return;
	}

	std::cout << "\nVeuillez ouvrir autant de chatrooms que vous le souhaitez" << std::endl;

	bind(server, (sockaddr*)&sin, sizeof(sin));
	listen(server, SOMAXCONN);
	FD_SET(server, &master);
}

void Server::ServerRoutine() const
{
	fd_set copy = master;

	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

	for (int i = 0; i < socketCount; i++) {
		SOCKET s = copy.fd_array[i];
		if (s == server) {
			//Réception de nouvelles chatroom
			SOCKET client = accept(server, nullptr, nullptr);
			FD_SET(client, &master);
			std::cout << "Nouvelle chatroom créée" << std::endl;
			std::string clName;
			WSM->RecieveMsg(client, clName);
			std::cout << "Bonjour " << clName << std::endl;
		}
	}
}

void Server::Close() const
{
	closesocket(server);
	FD_ZERO(&master);
}
