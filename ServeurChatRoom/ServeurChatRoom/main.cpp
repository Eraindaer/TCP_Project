#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>
#include <map>
#include <random>
#include <functional>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

std::string Compression(std::string sentence, std::map<char, std::string>& code) {
	std::string copy = sentence;

	std::multimap <int, char> letters;
	std::multimap <int, std::string> tree;
	std::multimap <int, std::string> tree2;

	std::sort(sentence.begin(), sentence.end());

	int iteration = 1;

	for (unsigned int i = 1; i < sentence.length(); i++) {
		if (sentence[i] == sentence[i - 1]) {
			iteration++;
		}
		else {
			letters.insert(std::make_pair(iteration, sentence[i - 1]));
			iteration = 1;
		}
	}
	letters.insert(std::make_pair(iteration, sentence[sentence.length() - 1]));
	for (auto const& entry : letters) {
		std::string s(1, entry.second);
		tree.insert(std::make_pair(entry.first, s));
		code.insert(std::make_pair(entry.second, ""));
	}

	int finalValue = 0;
	do {
		tree2.clear();
		int iterations = 0, value = 0;
		std::string strLetters = "";
		for (auto const& entry : tree) {
			if (iterations < 2) {
				strLetters += entry.second.c_str();
				value += entry.first;
				for (unsigned int i = 0; i < entry.second.length(); i++) {
					int binaryCode = (iterations == 0);
					code.find(entry.second[i])->second += std::to_string(binaryCode);
				}
			}
			else {
				tree2.insert(std::make_pair(entry.first, entry.second));
			}
			if (iterations == 1) {
				tree2.insert(std::make_pair(value, strLetters));
			}
			iterations++;
		}
		tree.clear();
		for (auto const& entry : tree2) {
			tree.insert(std::make_pair(entry.first, entry.second));
		}
	} while (tree.size() != 1);

	for (auto& entry : code) {
		std::reverse(entry.second.begin(), entry.second.end());
	}

	std::string encodedText;
	for (unsigned int i = 0; i < copy.length(); i++) {
		for (char c : code.find(copy[i])->second) {
			encodedText += c;
		}
	}
	return encodedText;
}

//Décompression
std::string Decompression(std::map<char, std::string> code, std::string encodedText) {
	std::string encodedChar = "", decompressedText = "";

	for (unsigned int i = 0; i < encodedText.size(); i++) {
		encodedChar += encodedText[i];
		for (auto& entry : code) {
			if (encodedChar == entry.second) {
				decompressedText += entry.first;
				encodedChar = "";
				break;
			}
		}
	}
	return decompressedText;
}

void SendMsg(const SOCKET& sock, std::string msg) {
	unsigned int msgSize = msg.size();
	if (send(sock, (char*)&msgSize, sizeof(int), 0) == SOCKET_ERROR)
		ExitProcess(EXIT_FAILURE);
	if (send(sock, msg.c_str(), msgSize, 0) == SOCKET_ERROR)
		ExitProcess(EXIT_FAILURE);
}

void RecieveMsg(const SOCKET& sock, std::string& msg) {
	int size;
	if (recv(sock, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		ExitProcess(EXIT_FAILURE);
	char* buffer = new char[size + 1];
	if (recv(sock, buffer, size, 0) == SOCKET_ERROR) {
		delete[]buffer;
		ExitProcess(EXIT_FAILURE);
	}
	if (size == 0)
		ExitProcess(EXIT_FAILURE);
	else
	{
		buffer[size] = '\0';
		std::string temp(buffer);
		msg = temp;
		msg.resize(size);
	}
	delete[]buffer;
}

int main(int argc, char* argv[]) {
	WSAData wsa;
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (err < 0)
		ExitProcess(EXIT_FAILURE);

#ifdef CLIENT

	std::string name = "client";
	int port;
	std::cout << "***LANCEMENT DU CLIENT***" << std::endl;

	std::cout << "\nVeuillez entrer le port indiqué sur l'une des chatrooms" << std::endl;
	std::cin >> port;
	std::cin.ignore();
	std::cout << "Veuillez entrer votre nom" << std::endl;
	std::getline(std::cin, name);

	while (name[0] == '/'){
		std::string temp = "";
		for (char c : name) {
			if (c != '/')
				temp += c;
		}
		name = temp;
	}

	SOCKET sock;
	SOCKADDR_IN sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	if (connect(sock, (sockaddr*)&sin, sizeof(sin)) != SOCKET_ERROR) {
		std::cout << "Client connecté" << std::endl;
		SendMsg(sock, name);
	}
	else
		ExitProcess(EXIT_FAILURE);
	std::map<char, std::string> code;

	std::thread sendThread([&sock, &name, &code]() {
		while (true) {
			code.clear();
			std::string test;
			std::getline(std::cin, test);
			if (test[0] == '/') {
				if (test == "/clear") {
					system("cls");
					std::cout << "***CHAT NETTOYE***\n\n";
				}
				else {
					SendMsg(sock, test);
				}
			}
			else {
				std::string test2 = name + " : ";
				test = test2 + test;
				SendMsg(sock, test);
			}
		}
		});
	sendThread.join();
	while (true) {
		std::string msg;
		RecieveMsg(sock, msg);
		
		std::cout << msg << std::endl;
	}
	
#endif
#ifdef CHATROOM
	std::cout << "***LANCEMENT DE LA CHATROOM***" << std::endl;
	SOCKET connectToServ;
	SOCKADDR_IN sin;

	connectToServ = socket(AF_INET, SOCK_STREAM, 0);
	if (connectToServ != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		sin.sin_port = htons(23);
		sin.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	std::string name = "chatroom";
	int port = 81;

	if (connect(connectToServ, (sockaddr*)&sin, sizeof(sin)) != SOCKET_ERROR) {
		std::cout << "***CONNEXION AU SERVER***" << std::endl;
	}
	else
		ExitProcess(EXIT_FAILURE);
	SendMsg(connectToServ, name);
	std::string strPort;
	RecieveMsg(connectToServ, strPort);
	port = std::atoi(strPort.c_str());
	std::cout << "Port attribué : " << port << std::endl;

	std::cout << "\nVeuillez ouvrir autant de clients que vous le souhaitez et inscrivez le port ci-dessus" << std::endl;

	SOCKET server;
	SOCKADDR_IN sinServ;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server != INVALID_SOCKET) {
		sinServ.sin_addr.S_un.S_addr = ADDR_ANY;
		sinServ.sin_port = htons(port);
		sinServ.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	bind(server, (sockaddr*)&sinServ, sizeof(sin));
	listen(server, SOMAXCONN);
	
	fd_set master;
	FD_ZERO(&master);

	FD_SET(server, &master);
	std::map <int, std::function<void(const std::string&, const SOCKET&)>> commandList;

	//*********** COMMANDS *****************\\
	// /clear defined in client
	// /? /help -> Give all commands
	commandList[0] = [](const std::string& msg, const SOCKET& outSock) {
		std::string serverMsg = "Liste des commandes : \n/clear -> Nettoie la fenêtre du client \n/say [something] -> permet d'afficher un texte pour tout le mondevia le serveur\n/TicTacToe -> permet d'exécuter le jeu TicTacToe\n/write [name] [text] -> Permet d'écrire le [text] dans un .txt avec le [name]\n/read [name] -> Permet de lire un .txt ayant le même nom que [name]\n/rng [number1] [number2] -> Affiche un nombre aléatoire entre [number1] et [number2]";
		SendMsg(outSock, serverMsg);
	};

	// /say [something] -> SERVER : [something]
	commandList[1] = [](const std::string& msg, const SOCKET& outSock) {
		std::string serverMsg = "SERVER : " + msg;
		SendMsg(outSock, serverMsg);
	};
	// /tictactoe -> Start TicTacToe.exe
	commandList[2] = [](const std::string& msg, const SOCKET& outSock) {
		std::string serverMsg = "SERVER : Lancement de " + msg;
		SendMsg(outSock, serverMsg);
		system("start ..\\TicTacToe\\TicTacToe\\TicTacToe.exe");
	};

	// /write [name] [text] -> Write the [text] in a .txt with the name [name]
	commandList[3] = [](const std::string& msg, const SOCKET& outSock) {
		std::string name = "";
		std::string text = "";
		bool space = false;
		for (char c : msg) {
			if (!space) {
				if (c == ' ')
					space = true;
				else
					name += c;
			}
			else
				text += c;
		}
		std::string serverMsg = "SERVER : Ecriture dans le fichier : " + name + ".txt";
		SendMsg(outSock, serverMsg);
		std::string cmd = "Echo " + text + " > \"" + name + ".txt\"";
		system(cmd.c_str());
	};
	
	// /read [name] -> Read the .txt with the name [name]
	commandList[4] = [](const std::string& msg, const SOCKET& outSock) {
		std::string serverMsg = "SERVER : Lecture du fichier : " + msg + ".txt";
		SendMsg(outSock, serverMsg);
		std::string cmd = ".\\" + msg + ".txt";
		system(cmd.c_str());
	};

	// /rng [number1] [number2] -> Gives a random number between [number1] and [number2]
	commandList[5] = [](const std::string& msg, const SOCKET& outSock) {
		int number1 = 0;
		int number2 = 0;
		bool error = false;
		bool space = false;
		std::string nbr1 = "";
		std::string nbr2 = "";
		for (char c : msg) {
			if (!space) {
				if (c == ' ')
					space = true;
				else if (std::isalpha(c)) {
					error = true;
					break;
				}
				else
					nbr1 += c;
			}
			else {
				if (std::isalpha(c)) {
					error = true;
					break;
				}
				else
					nbr2 += c;
			}
		}
		if (error) {
			SendMsg(outSock, "ERREUR FATALE : Arguments non numériques.\nVeuillez inscrire des arguments numériques");
		}
		else {
			number1 = std::stoi(nbr1);
			number2 = std::stoi(nbr2);

			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> dist(number1, number2);

			std::string rndNbr = std::to_string(dist(rng));

			SendMsg(outSock, "SERVER : Le dé est jeté ! Vous obtenez : " + rndNbr);
		}
	};
	//**************************************\\

	std::thread serverRoutine([&master, &server, &commandList]() {while (true) {
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET s = copy.fd_array[i];
			if (s == server) {
				SOCKET client = accept(server, nullptr, nullptr);
				FD_SET(client, &master);
				std::cout << "***CLIENT CONNECTE A LA CHATROOM***" << std::endl;
				std::string clName;
				RecieveMsg(client, clName);
				std::string welcome = "Bonjour " + clName;
				std::cout << welcome << std::endl;
				welcome = "SERVER : " + welcome;
				std::string msg = clName + " a rejoint la room";
				for (unsigned int i = 0; i < master.fd_count; i++) {
					SOCKET outSock = master.fd_array[i];
					if (outSock != server) {
						SendMsg(outSock, msg);
					}
				}
				SendMsg(client, welcome);
			}
			else {
				std::string msg;
				RecieveMsg(s, msg);
				if (msg[0] == '/') {
					std::string command = "";
					std::string argument0 = "";
					std::string argument1 = "";
					int space = 0;
					for (unsigned int i = 1; i < msg.size(); i++) {
						if (msg[i] == ' ')
							space++;
						else {
							if (space == 0)
								command += msg[i];
							else if (space == 1)
								argument0 += msg[i];
						}
						if (space >= 2)
							argument1 += msg[i];
					}
					if (command == "help" || command == "?") {
						if (argument0.size() != 0 || argument1.size() != 0)
							SendMsg(s, "ERREUR : Argument incorrect\nType d'erreur : Trop d'arguments");
						else {
							commandList[0]("", s);
						}
					}
					else if (command == "say") {
						if (argument0 == "")
							SendMsg(s, "ERREUR : Argument insuffisant\nType d'erreur : Oubli d'argument");
						else if (argument1 != "")
							SendMsg(s, "ERREUR : Arguments incorrects\nType d'erreur : Trop d'arguments");
						else {
							for (unsigned int i = 0; i < master.fd_count; i++) {
								SOCKET outSock = master.fd_array[i];
								if (outSock != server) {
									commandList[1](argument0, outSock);
								}
							}
						}
					}
					else if (command == "TicTacToe") {
						if (argument0 != "")
							SendMsg(s, "ERREUR : Argument incorrect\nType d'erreur : Trop d'arguments");
						else {
							commandList[2](command, s);
						}
					}
					else if (command == "write") {
						if (argument0 == "" || argument1 == "")
							SendMsg(s, "ERREUR : Arguments insuffisants\nType d'erreur : Oubli d'argument(s)\nRappel commande : /write [name] [text]");
						else {
							std::string msg = argument0 + " " + argument1;
							commandList[3](msg, s);
						}
					}
					else if (command == "read") {
						if (argument0 == "")
							SendMsg(s, "ERREUR : Argument insuffisant\nType d'erreur : Oubli d'argument\nRappel commande : /read [name]");
						else if (argument1 != "")
							SendMsg(s, "ERREUR : Arguments incorrects\nType d'erreur : Trop d'arguments\nRappel commande : /read [name]");
						else {
							commandList[4](argument0, s);
						}
					}
					else if (command == "rng") {
						if (argument0 == "" || argument1 == "")
							SendMsg(s, "ERREUR : Arguments Insuffisants\nType d'erreur : Oubli d'argument(s)\nRappel commande : /rng [number1] [number2]");
						else {
							std::string msg = argument0 + " " + argument1;
							commandList[5](msg, s);
						}
					}
					else
						SendMsg(s, "ERREUR : Commande inconnue");
					}
				else {
					std::map<char, std::string> code;
					std::string compressedMsg = Compression(msg, code);
					int bytesIn = msg.size();
					if (bytesIn <= 0) {
						closesocket(s);
						FD_CLR(s, &master);
					}
					else {
						std::cout << Decompression(code, compressedMsg) << std::endl;
						for (unsigned int i = 0; i < master.fd_count; i++) {
							SOCKET outSock = master.fd_array[i];
							if (outSock != server && outSock != s) {
								std::string uncompressedMsg = Decompression(code, compressedMsg);
								SendMsg(outSock, uncompressedMsg);
							}
						}
					}
				}
			}
		}
	}});
	serverRoutine.join();
#endif
#ifdef SERVER

	std::cout << "***LANCEMENT DU SERVEUR***" << std::endl;
	std::cout << "\nVeuillez ouvrir autant de chatrooms que vous le souhaitez" << std::endl;

	SOCKET server;
	SOCKADDR_IN sin;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = ADDR_ANY;
		sin.sin_port = htons(23);
		sin.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	bind(server, (sockaddr*)&sin, sizeof(sin));
	listen(server, SOMAXCONN);
	fd_set master;
	FD_ZERO(&master);

	FD_SET(server, &master);
	int roomPort = 24;

	while (true) {
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET s = copy.fd_array[i];
			if (s == server) {
				SOCKET client = accept(server, nullptr, nullptr);
				FD_SET(client, &master);
				std::cout << "Nouvelle chatroom créée" << std::endl;
				std::string clName;
				RecieveMsg(client, clName);
				std::cout << "Bonjour " << clName << std::endl;

				if (clName == "chatroom") {
					std::cout << "Port = " << roomPort << std::endl;
					std::string portStr = std::to_string(roomPort);
					SendMsg(client, portStr);
					std::cout << "port envoyé" << std::endl;
					roomPort++;
				}
			}
		}
	}
#endif
	WSACleanup();
	return 0;
}