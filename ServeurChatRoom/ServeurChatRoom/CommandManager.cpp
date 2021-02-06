#include "CommandManager.h"

void CommandManager::InitCommands(WinSockManager& WSM, const SOCKET& server, const fd_set& master, Logger& log, std::string& name)
{
	///***************************************Initialisation des commandes********************************\\\ 
	// /? /help -> Give all commands
	commandList["help"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 1) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string serverMsg = "Liste des commandes : \n/clear -> Nettoie la fenêtre du client \n/say [something] -> permet d'afficher un texte pour tout le mondevia le serveur\n/TicTacToe -> permet d'exécuter le jeu TicTacToe\n/write [name] [text] -> Permet d'écrire le [text] dans un .txt avec le [name]\n/read [name] -> Permet de lire un .txt ayant le même nom que [name]\n/delete [name] -> Permet de supprimer le .txt ayant le même nom que [name]\n/rng [number1] [number2] -> Affiche un nombre aléatoire entre [number1] et [number2]\n/roomname [name] -> Permet de changer le nom de la room pour [name]";
		log.WriteLine(">>" + serverMsg);
		WSM.SendMsg(outSock, serverMsg);
	};
	commandList["?"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 1) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string serverMsg = "Liste des commandes : \n/clear -> Nettoie la fenêtre du client \n/say [something] -> permet d'afficher un texte pour tout le mondevia le serveur\n/TicTacToe -> permet d'exécuter le jeu TicTacToe\n/write [name] [text] -> Permet d'écrire le [text] dans un .txt avec le [name]\n/read [name] -> Permet de lire un .txt ayant le même nom que [name]\n/rng [number1] [number2] -> Affiche un nombre aléatoire entre [number1] et [number2]";
		log.WriteLine(">>" + serverMsg);
		WSM.SendMsg(outSock, serverMsg);
	};

	// /say [something] -> SERVER : [something]
	commandList["say"] = [&WSM, &master, &server, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() < 2) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string text = "";
		for (unsigned int i = 1; i < args.size(); i++) {
			text += args[i] + " ";
		}
		std::string serverMsg = "SERVER : " + text;
		log.WriteLine(">>" + serverMsg);
		WSM.SendMsg(outSock, "Vous invoquez le serveur :");
		for (unsigned int i = 0; i < master.fd_count; i++) {
			SOCKET out = master.fd_array[i];
			if (out != server) {
				WSM.SendMsg(out, serverMsg);
			}
		}
	};
	// /tictactoe -> Start TicTacToe.exe
	commandList["tictactoe"] = [&WSM, &log](const std::vector<std::string> args, const SOCKET& outSock) {
		if (args.size() != 1) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string serverMsg = "SERVER : Lancement de TicTacToe";
		WSM.SendMsg(outSock, serverMsg);
		log.WriteLine(">>" + serverMsg);
		system("start ..\\TicTacToe\\TicTacToe\\TicTacToe.exe");
	};

	// /write [name] [text] -> Write the [text] in a .txt with the name [name]
	commandList["write"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() <= 2) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string text = "";
		for (unsigned int i = 2; i < args.size(); i++) {
			text += args[i] + " ";
		}

		std::string serverMsg = "SERVER : Ecriture dans le fichier : " + args[1] + ".txt";
		WSM.SendMsg(outSock, serverMsg);
		log.WriteLine(">>" + serverMsg);
		std::string cmd = "Echo " + text + " > \"" + args[1] + ".txt\"";
		system(cmd.c_str());
	};

	// /read [name] -> Read the .txt with the name [name]
	commandList["read"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 2) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string serverMsg = "SERVER : Lecture du fichier : " + args[1] + ".txt";
		WSM.SendMsg(outSock, serverMsg);
		log.WriteLine(">>" + serverMsg);
		std::string cmd = ".\\" + args[1] + ".txt";
		system(cmd.c_str());
	};

	// /delete [name] -> Delete the .txt with the name [name]
	commandList["delete"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 2) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string serverMsg = "SERVER : Suppression du fichier : " + args[1] + ".txt";
		WSM.SendMsg(outSock, serverMsg);
		log.WriteLine(">>" + serverMsg);
		std::string cmd = "del /f " + args[1] + ".txt";
		system(cmd.c_str());
	};

	// /rng [number1] [number2] -> Gives a random number between [number1] and [number2]
	commandList["rng"] = [&WSM, &log](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() != 3) {
			throw std::invalid_argument("Arguments invalides");
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
			throw std::invalid_argument("Arguments non numériques.\nVeuillez inscrire des arguments numériques");
			return;
		}
		else {
			std::random_device rd;
			std::mt19937 rng(rd());

			if (std::stoi(nbr[0]) > std::stoi(nbr[1])) {
				throw std::invalid_argument("Arguments dans le mauvais ordre.\nVeuillez les réinscrire");
				return;
			}
			std::uniform_int_distribution<int> dist(std::stoi(nbr[0]), std::stoi(nbr[1]));

			std::string rndNbr = std::to_string(dist(rng));

			WSM.SendMsg(outSock, "SERVER : Le dé est jeté ! Vous obtenez : " + rndNbr);
			log.WriteLine(">>SERVER : Le dé est jeté ! Vous obtenez : " + rndNbr);
		}
	};

	// /roomname [name] -> changes the room's name
	commandList["roomname"] = [&WSM, &master, &server, &log, &name](const std::vector<std::string>& args, const SOCKET& outSock) {
		if (args.size() < 2) {
			throw std::invalid_argument("Arguments invalides");
			return;
		}
		std::string newName = "";
		for (unsigned int i = 1; i < args.size(); i++) {
			newName += args[i] + " ";
		}
		std::string serverMsg = "SERVER : Changement du nom de la room pour : " + newName;
		log.WriteLine(">>" + serverMsg);
		name = newName;
		for (unsigned int i = 0; i < master.fd_count; i++) {
			SOCKET out = master.fd_array[i];
			if (out != server) {
				WSM.SendMsg(out, serverMsg);
			}
		}
	};
	///***************************************************************************************************\\\ 
}

void CommandManager::Execute(const std::vector<std::string>& args, const SOCKET& sock)
{
	commandList[args[0]](args, sock);
}
