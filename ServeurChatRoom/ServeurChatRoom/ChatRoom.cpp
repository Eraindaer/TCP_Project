#include "ChatRoom.h"

ChatRoom::ChatRoom(const WinSockManager& WSM)
{
	this->WSM = WSM;
	name = "chatroom"; 
	std::cout << "***LANCEMENT DE LA CHATROOM***" << std::endl;
	std::cout << "\nVeuillez entrer le port du serveur" << std::endl;
}

ChatRoom::~ChatRoom()
{
}

void ChatRoom::InitServerConnection(const int& port)
{
	SOCKET connectToServ;
	SOCKADDR_IN sinServ;

	connectToServ = socket(AF_INET, SOCK_STREAM, 0);
	if (connectToServ != INVALID_SOCKET) {
		sinServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		sinServ.sin_port = htons(port);
		sinServ.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);
	
	if(connect(connectToServ, (sockaddr*)&sinServ, sizeof(sinServ)) != SOCKET_ERROR) {
		std::cout << "***CONNEXION AU SERVER***" << std::endl;
	}
	else
	ExitProcess(EXIT_FAILURE);
	WSM.SendMsg(connectToServ, name);
	std::cout << "\nVeuillez entrer le port de la chatroom" << std::endl;
}

void ChatRoom::InitSocket(const int& port)
{
	std::cout << "\n***CHATROOM INITIALISEE : DISCUSSION POSSIBLE" << std::endl;
	std::cout << "Port : " << port << std::endl;

	SOCKADDR_IN sin;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server != INVALID_SOCKET) {
		sin.sin_addr.S_un.S_addr = ADDR_ANY;
		sin.sin_port = htons(port);
		sin.sin_family = AF_INET;
	}
	else
		ExitProcess(EXIT_FAILURE);

	bind(server, (sockaddr*)&sin, sizeof(sin));
	listen(server, SOMAXCONN);

	FD_ZERO(&master);
	FD_SET(server, &master);
}

void ChatRoom::InitCommands()
{
	WinSockManager w = WSM;
	SOCKET s = server;
	fd_set &m = master;
	//*********** COMMANDS *****************\\
	// /clear defined in client
	// -> Passer à class
	// Ajouter : Logger
	// - Classe client et chatroom
	// - Classe commandes
	//
	// Vecteur d'arguments
	// /? /help -> Give all commands
	commandList["help"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 1) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string serverMsg = "Liste des commandes : \n/clear -> Nettoie la fenêtre du client \n/say [something] -> permet d'afficher un texte pour tout le mondevia le serveur\n/TicTacToe -> permet d'exécuter le jeu TicTacToe\n/write [name] [text] -> Permet d'écrire le [text] dans un .txt avec le [name]\n/read [name] -> Permet de lire un .txt ayant le même nom que [name]\n/rng [number1] [number2] -> Affiche un nombre aléatoire entre [number1] et [number2]";
		w.SendMsg(outSock, serverMsg);
	};
	commandList["?"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 1) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string serverMsg = "Liste des commandes : \n/clear -> Nettoie la fenêtre du client \n/say [something] -> permet d'afficher un texte pour tout le mondevia le serveur\n/TicTacToe -> permet d'exécuter le jeu TicTacToe\n/write [name] [text] -> Permet d'écrire le [text] dans un .txt avec le [name]\n/read [name] -> Permet de lire un .txt ayant le même nom que [name]\n/rng [number1] [number2] -> Affiche un nombre aléatoire entre [number1] et [number2]";
		w.SendMsg(outSock, serverMsg);
	};

	// /say [something] -> SERVER : [something]
	commandList["say"] = [&w, &m, &s](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() < 2) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string text = "";
		for (unsigned int i = 1; i < args.size(); i++) {
			text += args[i] + " ";
		}
		std::string serverMsg = "SERVER : " + text;
		w.SendMsg(outSock, "Vous invoquez le serveur :");
		for (unsigned int i = 0; i < m.fd_count; i++) {
			SOCKET out = m.fd_array[i];
			if (out != s) {
				w.SendMsg(out, serverMsg);
			}
		}
	};
	// /tictactoe -> Start TicTacToe.exe
	commandList["tictactoe"] = [&w](const std::vector<std::string> args, const SOCKET& outSock) {
		if (args.size() != 1) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string serverMsg = "SERVER : Lancement de TicTacToe";
		w.SendMsg(outSock, serverMsg);
		system("start ..\\TicTacToe\\TicTacToe\\TicTacToe.exe");
	};

	// /write [name] [text] -> Write the [text] in a .txt with the name [name]
	commandList["write"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() <= 2) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string text = "";
		for (unsigned int i = 2; i < args.size(); i++) {
			text += args[i] + " ";
		}

		std::string serverMsg = "SERVER : Ecriture dans le fichier : " + args[1] + ".txt";
		w.SendMsg(outSock, serverMsg);
		std::string cmd = "Echo " + text + " > \"" + args[1] + ".txt\"";
		system(cmd.c_str());
	};

	// /read [name] -> Read the .txt with the name [name]
	commandList["read"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 2) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string serverMsg = "SERVER : Lecture du fichier : " + args[1] + ".txt";
		w.SendMsg(outSock, serverMsg);
		std::string cmd = ".\\" + args[1] + ".txt";
		system(cmd.c_str());
	};

	// /delete [name] -> Delete the .txt with the name [name]
	commandList["delete"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 2) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		std::string serverMsg = "SERVER : Suppression du fichier : " + args[1] + ".txt";
		w.SendMsg(outSock, serverMsg);
		std::string cmd = "del /f " + args[1] + ".txt";
		system(cmd.c_str());
	};

	// /rng [number1] [number2] -> Gives a random number between [number1] and [number2]
	commandList["rng"] = [&w](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 3) {
			w.SendMsg(outSock, "ERREUR : Arguments incorrects");
			return;
		}
		bool error = false;
		std::string nbr[2] = { "","" };
		for (unsigned int i = 1; i < args.size(); i++)
			if (!error)
				for (char c : args[i]) {
					if (std::isalpha(c)) {
						error = true;
						break;
					}
					else
						nbr[i - 1] += c;
				}
		if (error) {
			w.SendMsg(outSock, "ERREUR FATALE : Arguments non numériques.\nVeuillez inscrire des arguments numériques");
		}
		else {
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> dist(std::stoi(nbr[0]), std::stoi(nbr[1]));

			std::string rndNbr = std::to_string(dist(rng));

			w.SendMsg(outSock, "SERVER : Le dé est jeté ! Vous obtenez : " + rndNbr);
		}
	};
}

void ChatRoom::RoutineChatRoom()
{
	fd_set copy = master;
	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
	for (unsigned int i = 0; i < socketCount; i++) {
		SOCKET sock = copy.fd_array[i];
		if (sock == server) {
			SOCKET client = accept(server, nullptr, nullptr);
			FD_SET(client, &master);
			std::cout << "***CLIENT CONNECTE A LA CHATROOM***" << std::endl;
			std::string clName;
			WSM.RecieveMsg(client, clName);
			std::string welcome = "Bonjour " + clName;
			std::cout << welcome << std::endl;
			welcome = "SERVER : " + welcome;
			welcome += "\nSi vous souhaitez voir la liste des commandes, tapez \"/help\" ou \"/?\"";
			std::string msg = clName + " a rejoint la room";
			for (unsigned int i = 0; i < master.fd_count; i++) {
				SOCKET outSock = master.fd_array[i];
				if (outSock != server) {
					WSM.SendMsg(outSock, msg);
				}
			}
			WSM.SendMsg(client, welcome);
		}
		else {
			std::string msg;
			WSM.RecieveMsg(sock, msg);
			if (msg[0] == '/') {
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
					commandList[args[0]](args, sock);
				}
				catch (std::exception& ex) {
					WSM.SendMsg(sock, "ERREUR : Commande inconnue");
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
