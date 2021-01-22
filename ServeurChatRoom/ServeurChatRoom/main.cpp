#include "Server.h"
#include "ChatRoom.h"
#include "Client.h"
#include <thread>

int main(int argc, char* argv[]) {
	WinSockManager WSM;

#ifdef CLIENT

	Client client(WSM);
	bool isConnected = false;
	while (!isConnected) {
		int port;
		std::cin >> port;
		std::cin.ignore();
		std::string name;
		std::getline(std::cin, name);
		try {
			client.InitSocket(port, name, isConnected);
		}
		catch (std::exception& ex) {
			std::cout << "ERREUR : " << ex.what() << std::endl;
			std::cout << "Veuillez réinscrire votre nom et le port" << std::endl;
		}
	}
	std::thread sendThread([&client]() { while (true) { client.SendingClient();}});
	while (true) {
		client.ReceivingClient();
	}
	sendThread.join();

#endif
#ifdef CHATROOM
	ChatRoom chatRoom(WSM);

	int serverPort;
	std::cin >> serverPort;
	try {
		chatRoom.InitServerConnection(serverPort);
	}
	catch (int& e) {
		std::cout << "Session terminée. Code d'erreur : " << e << std::endl;
		ExitProcess(EXIT_FAILURE);
	}
	std::cin.ignore();
	int chatRoomPort;
	std::cin >> chatRoomPort;
	try {
		chatRoom.InitSocket(chatRoomPort);
	}
	catch (int& e) {
		std::cout << "Session terminée. Code d'erreur : " << e << std::endl;
		ExitProcess(EXIT_FAILURE);
	}
	std::thread serverRoutine([&chatRoom]() { while (true) { chatRoom.RoutineChatRoom(); }});
	serverRoutine.join();
#endif
#ifdef SERVER

	Server server(WSM);
	
	int port;
	std::cin >> port;

	try {
		server.InitSocket(port);
	}
	catch (std::exception& ex) {
		std::cout << "Erreur : " << ex.what() << std::endl;
		ExitProcess(EXIT_FAILURE);
	}
	while (true) {
		server.ServerRoutine();
	}
#endif
	WSM.~WinSockManager();
	return 0;
}