#pragma once
#include "WinSockManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

class Logger
{
public:
	Logger();
	~Logger() = default;
	Logger(const Logger&);
	Logger(Logger&&) = default;
	Logger& operator=(const Logger&);
	Logger& operator=(Logger&&) = default;

	void AddClient(const std::string& name);

	void WriteLine(const std::string& line);
	void WriteError(const std::string& line, const std::exception& e);

	void Save();

private:
	std::vector<std::string>	clientList;
	std::ofstream				file;
	std::ofstream				errorFile;
	std::string					fileName;
	std::string					errorFileName;
};

