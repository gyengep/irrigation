#include "CommandExecutor.h"
#include "Command.h"
#include "Exceptions.h"

using namespace std;


CommandExecutor::CommandExecutor() {
}

CommandExecutor::~CommandExecutor() {
	lock_guard<std::mutex> lock(mutex);

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		delete *it;
	}

	commands.clear();
}

void CommandExecutor::addCommand(Command* command) {

	if (nullptr == command) {
		throw invalid_argument("addCommand(): Invalid command");
	}

	lock_guard<std::mutex> lock(mutex);
	commands.push_back(command);
}

Command* CommandExecutor::getMatchingCommand(const string& commandText) const {

	if (commandText.empty()) {
		throw invalid_argument("getMatchingCommand(): Empty command");
	}

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		Command* command = *it;

		if (command->getCommand() == commandText) {
			return command;
		}
	}

	throw UnknownCommandException(commandText);
}

void CommandExecutor::execute(const Tokens& tokens) const {

	if (tokens.empty()) {
		throw invalid_argument("execute(): Invalid tokens");
	}

	lock_guard<std::mutex> lock(mutex);

	const Tokens parameters(++tokens.begin(), tokens.end());
	const string commandText(tokens.front());

	Command* cmd = getMatchingCommand(commandText);
	cmd->execute(parameters);
}
