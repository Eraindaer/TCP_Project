#pragma once
#include "WinSockManager.h"
#include "Logger.h"
#include <functional>
#include <random>
#include <vector>

class CommandManager
{
public:
	CommandManager() = default;

	void InitCommands(WinSockManager& WSM, const SOCKET& server, const fd_set& master, Logger& log, std::string& name);
	void Execute(const std::vector<std::string>& args, const SOCKET& sock);

private:
	std::map <std::string, std::function<void(const std::vector<std::string>&, const SOCKET&)>> commandList;
};

