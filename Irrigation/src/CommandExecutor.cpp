/*
 * CommandExecutor.cpp
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#include "common.h"
#include "CommandExecutor.h"

#include <climits>


CommandExecutor::CommandExecutor() {
}

CommandExecutor::~CommandExecutor() {
	std::lock_guard<std::mutex> lock(mtx);

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		delete *it;
	}

	commands.clear();
}

void CommandExecutor::execute(const Tokens& tokens) const throw (CommandLineException) {
	Tokens tokensCopy(tokens);
	if (!tokensCopy.empty()) {
		std::lock_guard<std::mutex> lock(mtx);
		Command& cmd = getCommand(tokensCopy);
		cmd.execute(tokensCopy);
	}
}

void CommandExecutor::addCommand(Command* command) {
	std::lock_guard<std::mutex> lock(mtx);
	commands.push_back(command);
}

Command& CommandExecutor::getCommand(Tokens& tokens) const {
	bool commandFound = false;
	std::string command = tokens.front();
	tokens.erase(tokens.begin());

	std::string subCommand;

	if (!tokens.empty()) {
		subCommand = tokens.front();
	}

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		if ((*it)->getCommand() == command) {
			commandFound = true;
			if ((*it)->getSubCommand().empty()) {
				return **it;
			} else {
				if (subCommand.empty()) {
					throw SubcommandMissingException();
				}

				if (subCommand == (*it)->getSubCommand()) {
					tokens.erase(tokens.begin());
					return **it;
				}
			}
		}
	}

	if (commandFound) {
		throw UnknownSubcommandException(subCommand);
	} else {
		throw UnknownCommandException(command);
	}
}

IdType Command::parseId(const std::string& text, const char* errorMessage) throw (CommandLineException) {
	return static_cast<IdType>(parseUInt(text, errorMessage));
}

unsigned Command::parseUInt(const std::string& text, const char* errorMessage) throw (CommandLineException) {
	std::size_t pos;
	unsigned long result;
	try {
		result = std::stoul(text, &pos, 10);
	} catch (std::invalid_argument& e) {
		throw CommandLineException(errorMessage);
	}

	if (text.length() != pos || result > UINT_MAX) {
		throw CommandLineException(errorMessage);
	}
	return result;
}

bool Command::parseOnOff(const std::string& text, const char* errorMessage) throw (CommandLineException) {
	bool result;

	if (text == "on") {
		result = true;
	} else if (text == "off") {
		result = false;
	} else {
		throw CommandLineException(errorMessage);
	}

	return result;
}

