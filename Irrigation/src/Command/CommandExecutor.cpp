#include "Common.h"
#include "CommandExecutor.h"


CommandExecutor::CommandExecutor() {
}

CommandExecutor::~CommandExecutor() {
	std::lock_guard<std::mutex> lock(mtx);

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		delete *it;
	}

	commands.clear();
}

void CommandExecutor::addCommand(Command* command) {
	std::lock_guard<std::mutex> lock(mtx);
	commands.push_back(command);
}

Command* CommandExecutor::getMatchingCommand(const std::string& commandText) const {
	for (auto it = commands.begin(); commands.end() != it; ++it) {
		if ((*it)->getCommand() == commandText) {
			return *it;
		}
	}

	throw UnknownCommandException(commandText);
}

void CommandExecutor::execute(const Tokens& tokens) const {
	std::lock_guard<std::mutex> lock(mtx);

	if (!tokens.empty()) {
		Tokens tokensCopy(tokens);

		Command* cmd = getMatchingCommand(tokensCopy.front());
		tokensCopy.erase(tokensCopy.begin());
		cmd->execute(tokensCopy);
	}
}
