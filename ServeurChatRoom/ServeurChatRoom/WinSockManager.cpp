#include "WinSockManager.h"

WinSockManager::WinSockManager()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		ExitProcess(EXIT_FAILURE);
}

WinSockManager::~WinSockManager()
{
	WSACleanup();
}

void WinSockManager::SendMsg(const SOCKET& sock, const std::string& msg)
{
	std::map<char, std::string> code;
	std::string compressedMsg = Compression(msg, code);

	//Envoi taille du message initial
	int msgSize = msg.size();
	if (send(sock, (char*)&msgSize, sizeof(int), 0) == SOCKET_ERROR)
		return;

	//Envoi message compressé avec sa taille
	int compressedMsgSize = compressedMsg.size();
	if (send(sock, (char*)&compressedMsgSize, sizeof(int), 0) == SOCKET_ERROR)
		return;
	if (send(sock, compressedMsg.c_str(), compressedMsgSize, 0) == SOCKET_ERROR)
		return;

	//"Conversion" map->string 
	std::string codeString = "";
	for (auto& entry : code) {
		codeString += entry.first + entry.second;
	}
	//Envoi clé
	int sizeOfKey = codeString.size();
	if (send(sock, (char*)&sizeOfKey, sizeof(int), 0) == SOCKET_ERROR)
		return;
	if (send(sock, codeString.c_str(), sizeOfKey, 0) == SOCKET_ERROR)
		return;
}

void WinSockManager::RecieveMsg(const SOCKET& sock, std::string& msg)
{
	//Réception taille message initial
	int size;
	if (recv(sock, reinterpret_cast<char*>(&size), sizeof(int), 0) == SOCKET_ERROR)
		return;

	//Réception message compressé
	int compressedSize;
	if (recv(sock, reinterpret_cast<char*>(&compressedSize), sizeof(int), 0) == SOCKET_ERROR)
		return;
	char* buffer = new char[compressedSize + 1];
	if (recv(sock, buffer, compressedSize, 0) == SOCKET_ERROR) {
		delete[]buffer;
		return;
	}
	//Réception clé
	unsigned int sizeOfKey;
	if (recv(sock, reinterpret_cast<char*>(&sizeOfKey), sizeof(unsigned int), 0) == SOCKET_ERROR)
		return;
	std::map<char, std::string> code;
	char* bufferKey = new char[sizeOfKey + 1];
	if (recv(sock, bufferKey, sizeOfKey, 0) == SOCKET_ERROR) {
		delete[]bufferKey;
		delete[]buffer;
		return;
	}
	std::string codeString;
	if (sizeOfKey == 0)
		return;
	else {
		bufferKey[sizeOfKey] = '\0';
		std::string temp(bufferKey);
		codeString = temp;
		codeString.resize(sizeOfKey);
		delete[]bufferKey;
	}

	//"Conversion" string->map
	char letter = codeString[0];
	std::string compressedLetter = "";
	for (unsigned int i = 1; i < codeString.size(); i++) {
		if (codeString[i] == '0' || codeString[i] == '1')
			compressedLetter += codeString[i];
		else {
			code.insert(std::make_pair(letter, compressedLetter));
			letter = codeString[i];
			compressedLetter = "";
		}
	}
	code.insert(std::make_pair(letter, compressedLetter));

	//Décompression du message
	if (size == 0)
		return;
	else
	{
		buffer[compressedSize] = '\0';
		std::string temp(buffer);
		msg = Decompression(code, temp);
		msg.resize(size);
	}
	delete[]buffer;
}

std::string WinSockManager::Compression(std::string sentence, std::map<char, std::string>& code)
{
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
	while (tree.size() != 1) {
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
	}

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

std::string WinSockManager::Decompression(std::map<char, std::string> code, std::string encodedText)
{
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
