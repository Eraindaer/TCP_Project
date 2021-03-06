#include "Logger.h"

Logger::Logger()
{
	time_t now = time(0);
	std::tm* ltm = localtime(&now);
	
	//Nom du fichier log : Ann�eMoisJourHeureMinuteSeconde
	std::string baseName = std::to_string(ltm->tm_year + 1900) + std::to_string(ltm->tm_mon + 1) + std::to_string(ltm->tm_hour) + std::to_string(ltm->tm_min) + std::to_string(ltm->tm_sec);
	fileName = "logs/" + baseName + ".txt";
	errorFileName = "logs/errors/" + baseName + ".txt";
	file.open(fileName.c_str());
	errorFile.open(errorFileName.c_str());
}

Logger::~Logger()
{
	Save();
}

Logger::Logger(const Logger& newLogger)
{
	clientList = newLogger.clientList;
	Save();
	file.open(newLogger.fileName);
	errorFile.open(newLogger.errorFileName);
	fileName = newLogger.fileName;
	errorFileName = newLogger.errorFileName;
}

Logger& Logger::operator=(const Logger& logAssign)
{
	clientList = logAssign.clientList;
	Save();
	file.open(logAssign.fileName);
	errorFile.open(logAssign.errorFileName);
	fileName = logAssign.fileName;
	errorFileName = logAssign.errorFileName;
	return *this;
}

void Logger::AddClient(const std::string & name)
{
	for (std::string& c : clientList) {
		if (name == c) {
			throw std::invalid_argument("Ce nom est d�j� pris");
			return;		
		}
	}
	clientList.push_back(name);
}

void Logger::WriteLine(const std::string& line)
{
	time_t now = time(0);
	std::tm* ltm = localtime(&now);
	std::string timeStamp = "[" + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec) + "] : ";
	file << timeStamp << line << "\n";
}

void Logger::WriteError(const std::string& line, const std::exception& e)
{
	time_t now = time(0);
	std::tm* ltm = localtime(&now);
	std::string timeStamp = "[" + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec) + "] : ";
	errorFile << timeStamp << line << e.what() << "\n";
}

void Logger::Save()
{
	file.close();
	errorFile.close();
}
