#include "Client.h"

Client::Client(const WinSockManager& WSM)
{
	this->WSM = WSM;
	sock = NULL;
	std::cout << "***LANCEMENT DU CLIENT***" << std::endl;
	std::cout << "\nVeuillez entrer le port indiqué sur l'une des chatrooms ainsi que votre nom" << std::endl;
}

Client::~Client()
{
	closesocket(sock);
}

void Client::InitSocket(const int& port, const std::string& name, bool& isConnected)
{
	this->name = name;
	while (this->name[0] == '/') {
		std::string temp = "";
		for (char c : name) {
			if (c != '/')
				temp += c;
		}
		this->name = temp;
	}
	SOCKADDR_IN sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else {
		throw std::invalid_argument("Impossible d'initialiser la session. Fin de la session");
		return;
	}
	if (connect(sock, (sockaddr*)&sin, sizeof(sin)) != SOCKET_ERROR) {
		WSM.SendMsg(sock, name);
		std::string returnValue;
		WSM.RecieveMsg(sock, returnValue);
		if (returnValue == "-1") {
			throw std::invalid_argument("Nom déjà pris. Veuillez en choisir un autre");
			return;
		}
		std::cout << "Client connecté" << std::endl;
		std::cout << "Votre nom : " << name << std::endl;
		std::cout << "Le port que vous avez choisi : " << port << std::endl;
	}
	else {
		throw std::invalid_argument("Impossible de se connecter. Fin de la session");
		return;
	}
	isConnected = true;
}

void Client::SendingClient()
{
	std::string test;
	std::getline(std::cin, test);
	if (test[0] == '/') {
		if (test == "/clear") {
			system("cls");
			std::cout << "***CHAT NETTOYE***\n\n";
		}
		else {
			WSM.SendMsg(sock, test);
		}
	}
	else {
		std::string test2 = name + " : ";
		test = test2 + test;
		WSM.SendMsg(sock, test);
	}
}

void Client::ReceivingClient()
{
	std::string msg;
	WSM.RecieveMsg(sock, msg);
	std::cout << msg << std::endl;
}
