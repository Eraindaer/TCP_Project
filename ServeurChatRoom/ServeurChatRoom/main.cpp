#include "Server.h"
#include "ChatRoom.h"
#include "Client.h"
#include <thread>

int main(int argc, char* argv[]) {
	WinSockManager WSM;
#ifdef CLIENT
	//Initialisation du client
	std::unique_ptr<Client> client = std::make_unique<Client>(WSM);
	bool isConnected = false;
	while (!isConnected) {
		int port;
		std::cin >> port;
		std::cin.ignore();
		std::string name;
		std::getline(std::cin, name);

		//Initialisation du socket
		TryCatchCheck([&client, &port, &name, &isConnected]() {client->InitSocket(port, name, isConnected); });
	}

	//Routine Envoi messages
	std::thread sendThread([&client]() { while (true) { client->SendingClient();}});

	//Réception messages
	while (true) {
		client->ReceivingClient();
	}
	sendThread.join();
#endif
#ifdef CHATROOM
	//Initialisation de la chatroom
	std::unique_ptr<ChatRoom> chatRoom = std::make_unique<ChatRoom>(WSM);
	int serverPort;
	std::cin >> serverPort;

	//Connexion au serveur général
	TryCatchCheck([&chatRoom, &serverPort]() {chatRoom->InitServerConnection(serverPort); });
	bool isOpen = false;
	while (!isOpen) {
		std::cin.ignore();
		int chatRoomPort;
		std::cin >> chatRoomPort;
		//Initialisation socket
		TryCatchCheck([&chatRoom, &chatRoomPort, &isOpen]() {chatRoom->InitSocket(chatRoomPort, isOpen); });
	}

	//Routine Chatroom
	std::thread serverRoutine([&chatRoom]() { while (true) { chatRoom->RoutineChatRoom(); }});
	serverRoutine.join();
#endif
#ifdef SERVER
	//Initialisation du serveur
	std::unique_ptr<Server> server = std::make_unique<Server>(WSM); //ptr ?"
	int port;
	std::cin >> port;

	//Initialisation socket
	TryCatchCheck([&server, &port]() {server->InitSocket(port);});

	//Routine Serveur
	while (true) {
		server->ServerRoutine();
	}
#endif
	WSM.Close();
	return 0;
}