#include "ChatRoom.h"

ChatRoom::ChatRoom(const WinSockManager& WSM)
{
	this->WSM = WSM;
	name = "chatroom"; 
	std::cout << "***LANCEMENT DE LA CHATROOM***" << std::endl;
	logger.WriteLine("***LANCEMENT DE LA CHATROOM***");
	std::cout << "\nVeuillez entrer le port du serveur" << std::endl;
	logger.WriteLine("\nVeuillez entrer le port du serveur");
}

ChatRoom::~ChatRoom()
{
	closesocket(server);
	logger.Save();
}

void ChatRoom::InitServerConnection(const int& port)
{
	serverPort = port;

	SOCKET connectToServer;
	SOCKADDR_IN sinServ;

	connectToServer = socket(AF_INET, SOCK_STREAM, 0);
	try {
		if (connectToServer != INVALID_SOCKET) {
			sinServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			sinServ.sin_port = htons(port);
			sinServ.sin_family = AF_INET;
		}
		else {
			throw std::invalid_argument("Impossible d'initialiser la session. Fin de la session");
		}
	}
	catch (std::exception& e) {
		errorHandler.HandleExceptionChatRoom(e, logger);
		throw 10;
		return;
	}
	
	try {
		if (connect(connectToServer, (sockaddr*)&sinServ, sizeof(sinServ)) != SOCKET_ERROR) {
			std::cout << "***CONNEXION AU SERVER***" << std::endl;
			logger.WriteLine("***CONNEXION AU SERVER***");
		}
		else {
			throw std::invalid_argument("Impossible de se connecter au serveur. Fin de la session");
		}
	}
	catch (std::exception& e) {
		errorHandler.HandleExceptionChatRoom(e, logger);
		throw 45;
		return;
	}
	WSM.SendMsg(connectToServer, name);
	logger.WriteLine(">>" + name);
	std::cout << "\nVeuillez entrer le port de la chatroom" << std::endl;
	logger.WriteLine("\nVeuillez entrer le port de la chatroom");
}

void ChatRoom::InitSocket(const int& port)
{
	try {
		if (port == serverPort) {
			throw std::invalid_argument("Impossible d'initialiser le salon. Fin de la session");
		}
	}
	catch (std::exception& e) {
		errorHandler.HandleExceptionChatRoom(e, logger);
		throw 22;
		return;
	}

	SOCKADDR_IN sin;
	server = socket(AF_INET, SOCK_STREAM, 0);
	try {
		if (server != INVALID_SOCKET) {
			sin.sin_addr.S_un.S_addr = ADDR_ANY;
			sin.sin_port = htons(port);
			sin.sin_family = AF_INET;
		}
		else {
			throw std::invalid_argument("Impossible d'initialiser le salon. Fin de la session");
		}
	}
	catch (std::exception& e) {
		errorHandler.HandleExceptionChatRoom(e, logger);
		throw 10;
		return;
	}

	std::cout << "\n***CHATROOM INITIALISEE : DISCUSSION POSSIBLE***" << std::endl;
	std::cout << "Port : " << port << std::endl;
	logger.WriteLine("\n***CHATROOM INITIALISEE : DISCUSSION POSSIBLE***");
	logger.WriteLine("Port : " + port);

	bind(server, (sockaddr*)&sin, sizeof(sin));
	listen(server, SOMAXCONN);

	FD_ZERO(&master);
	FD_SET(server, &master);

	commandManager.InitCommands(WSM, server, master, logger, name);
}

void ChatRoom::RoutineChatRoom()
{
	fd_set copy = master;
	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
	for (int i = 0; i < socketCount; i++) {
		SOCKET sock = copy.fd_array[i];
		if (sock == server) {
			SOCKET client = accept(server, nullptr, nullptr);
			std::string clName;
			WSM.RecieveMsg(client, clName);
			try {
				logger.AddClient(clName);
			}
			catch (std::exception& e) {
				WSM.SendMsg(client, "-1");
				logger.WriteError("ERROR : ", e);
				closesocket(client);
				return;
			}
			WSM.SendMsg(client, "0");
			FD_SET(client, &master);
			std::cout << "***CLIENT CONNECTE A LA CHATROOM***" << std::endl;
			logger.WriteLine("***CLIENT CONNECTE A LA CHATROOM***:\n");
			std::string welcome = "Bonjour " + clName;
			std::cout << welcome << std::endl;
			welcome = "SERVER : " + welcome;
			welcome += "\nBienvenue dans " + name;
			welcome += "\nSi vous souhaitez voir la liste des commandes, tapez \"/help\" ou \"/?\"";
			std::string msg = clName + " a rejoint la room";
			for (unsigned int i = 0; i < master.fd_count; i++) {
				SOCKET outSock = master.fd_array[i];
				if (outSock != server) {
					WSM.SendMsg(outSock, msg);
				}
			}
			WSM.SendMsg(client, welcome);
			logger.WriteLine(">>" + msg);
			logger.WriteLine(">>" + welcome);
		}
		else {
			std::string msg;
			WSM.RecieveMsg(sock, msg);
			if (msg[0] == '/') {
				logger.WriteLine("<<" + msg);
				std::string word = "";
				std::vector<std::string> args;
				for (unsigned int i = 1; i < msg.size(); i++) {
					if (msg[i] == ' ' || msg[i] == '\0') {
						args.push_back(word);
						word = "";
					}
					else
						word += msg[i];
				}
				args.push_back(word);
				try {
					commandManager.Execute(args, sock);
				}
				catch (std::exception& e) {
					errorHandler.HandleExceptionClient(WSM, e, sock, logger);
				}
			}
			else {
				std::map<char, std::string> code;
				//std::string compressedMsg = Compression(msg, code);
				int bytesIn = msg.size();
				if (bytesIn <= 0) {
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					std::cout << msg << std::endl;
					logger.WriteLine(msg);
					for (unsigned int i = 0; i < master.fd_count; i++) {
						SOCKET outSock = master.fd_array[i];
						if (outSock != server && outSock != sock) {
							//std::string uncompressedMsg = Decompression(code, compressedMsg);
							WSM.SendMsg(outSock, msg);
						}
					}
				}
			}
		}
	}
}
