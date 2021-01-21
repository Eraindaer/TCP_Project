#include "Server.h"
#include "ChatRoom.h"
#include "Client.h"

int main(int argc, char* argv[]) {
	WinSockManager WSM;
#ifdef CLIENT

	Client client(WSM);
	int port;
	try {
		std::cin >> port;
	}
	catch (std::exception& ex) {
		std::cout << "ERREUR : port non-numérique" << std::endl;
		ExitProcess(EXIT_FAILURE);
	}
	std::cin.ignore();
	std::string name;
	std::getline(std::cin, name);
	client.InitSocket(port, name);

	std::thread sendThread([&client]() {while (true) {
		client.SendingClient();
	}});
	while (true) {
		client.ReceivingClient();
	}
	sendThread.join();

#endif
#ifdef CHATROOM
	ChatRoom chatRoom(WSM);

	int serverPort;
	std::cin >> serverPort;
	chatRoom.InitServerConnection(serverPort);
	std::cin.ignore();
	int chatRoomPort;
	std::cin >> chatRoomPort;
	chatRoom.InitSocket(chatRoomPort);

	chatRoom.InitCommands();
	std::thread serverRoutine([&chatRoom]() {while (true) {
		chatRoom.RoutineChatRoom();
	}});
	serverRoutine.join();
#endif
#ifdef SERVER

	Server server(WSM);
	
	int port;
	try {
		std::cin >> port;
	}
	catch (std::exception& ex) {
		std::cout << "ERREUR : port non-numérique" << std::endl;
		ExitProcess(EXIT_FAILURE);
	}
	server.InitSocket(port);
	while (true) {
		server.ServerRoutine();
	}
#endif
	WSM.~WinSockManager();
	return 0;
}
