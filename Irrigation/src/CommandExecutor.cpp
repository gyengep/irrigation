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
}

void CommandExecutor::execute(Tokens& tokens) {
	if (!tokens.empty()) {
		try {
			std::lock_guard<std::mutex> lock(mtx);
			Command& cmd = getCommand(tokens);
			cmd.execute(tokens);

		} catch (CommandLineException& e) {
			onExecutionFailed(e);
		} catch (std::exception& e) {
			onError(e);
		}
	}
}

void CommandExecutor::addCommand(Command* command) {
	std::lock_guard<std::mutex> lock(mtx);
	commands.push_back(command);
}

Command& CommandExecutor::getCommand(Tokens& tokens) {
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
				if (subCommand == (*it)->getSubCommand()) {
					tokens.erase(tokens.begin());
					return **it;
				}
			}
		}
	}

	if (commandFound) {
		throw UnknownSubcommandException();
	} else {
		throw UnknownCommandException(command);
	}
}

IdType CommandExecutor::parseId(const std::string& text, const char* errorMessage) {
	return static_cast<IdType>(CommandExecutor::parseUInt(text, errorMessage));
}

unsigned CommandExecutor::parseUInt(const std::string& text, const char* errorMessage) {
	std::size_t pos;
	unsigned long result =std::stoul(text, &pos, 10);
	if (text.length() != pos || result > UINT_MAX) {
		throw CommandLineException(errorMessage);
	}
	return result;
}

bool CommandExecutor::parseOnOff(const std::string& text, const char* errorMessage) {
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

