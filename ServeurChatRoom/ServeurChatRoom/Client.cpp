#include "Client.h"

Client::Client()
{
	sock = NULL;
	std::cout << "***LANCEMENT DU CLIENT***" << std::endl;
	std::cout << "\nVeuillez entrer le port indiqu� sur l'une des chatrooms ainsi que votre nom" << std::endl;
}

Client::~Client()
{
	Close();
	WSM->Close();
}

Client::Client(const Client& newClient)
{
	WSM = std::make_unique<WinSockManager>(*newClient.WSM);
	name = newClient.name;
	sock = NULL;
	sock = newClient.sock;
}

Client& Client::operator=(const Client& copyAssign)
{
	WSM = std::make_unique<WinSockManager>(*copyAssign.WSM);
	name = copyAssign.name;
	closesocket(sock);
	sock = NULL;
	sock = copyAssign.sock;
	return *this;
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

	//V�rification socket client
	if (sock != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else {
		throw 16;
		return;
	}
	//Tentative de connexion et v�rification nom client
	if (connect(sock, (sockaddr*)&sin, sizeof(sin)) != SOCKET_ERROR) {
		WSM->SendMsg(sock, name);
		std::string returnValue;
		WSM->RecieveMsg(sock, returnValue);
		if (returnValue == "-1") {
			throw 21;
			return;
		}
		std::cout << "Client connect�" << std::endl;
		std::cout << "Votre nom : " << name << std::endl;
		std::cout << "Le port que vous avez choisi : " << port << std::endl;
	}
	else {
		throw 17;
		return;
	}
	isConnected = true;
}

void Client::SendingClient() const
{
	std::string test;
	std::getline(std::cin, test);
	if (test[0] == '/') {
		if (test == "/clear") {
			system("cls");
			std::cout << "***CHAT NETTOYE***\n\n";
		}
		else {
			WSM->SendMsg(sock, test);
		}
	}
	else {
		std::string test2 = name + " : ";
		test = test2 + test;
		WSM->SendMsg(sock, test);
	}
}

void Client::ReceivingClient() const
{
	std::string msg;
	WSM->RecieveMsg(sock, msg);
	std::cout << msg << std::endl;
}

void Client::Close() const {
	closesocket(sock);
}