/*
 * CommandExecutor.cpp
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#include "Common.h"
#include "CommandExecutor.h"

#include <string>


CommandExecutor::CommandExecutor() {
}

CommandExecutor::~CommandExecutor() {
	std::lock_guard<std::mutex> lock(mtx);

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		delete *it;
	}

	commands.clear();
}

void CommandExecutor::execute(const Tokens& tokens) const throw (CommandException) {
	Tokens tokensCopy(tokens);
	if (!tokensCopy.empty()) {
		std::lock_guard<std::mutex> lock(mtx);
		Command& cmd = getMatchingCommand(tokensCopy);
		cmd.execute(tokensCopy);
	}
}

void CommandExecutor::addCommand(Command* command) {
	std::lock_guard<std::mutex> lock(mtx);
	commands.push_back(command);
}

Command& CommandExecutor::getMatchingCommand(Tokens& tokens) const {
	bool commandFound = false;
	std::string commandText = tokens.front();
	tokens.erase(tokens.begin());

	std::string subCommandText;

	if (!tokens.empty()) {
		subCommandText = tokens.front();
	}

	for (auto it = commands.begin(); commands.end() != it; ++it) {
		if ((*it)->getCommand() == commandText) {
			commandFound = true;
			if ((*it)->getSubCommand().empty()) {
				return **it;
			} else {
				if (subCommandText == (*it)->getSubCommand()) {
					tokens.erase(tokens.begin());
					return **it;
				}
			}
		}
	}

	if (commandFound) {
		if (subCommandText.empty()) {
			throw SubcommandMissingException();
		} else {
			throw UnknownSubcommandException(subCommandText);
		}
	} else {
		throw UnknownCommandException(commandText);
	}
}

IdType Command::parseProgramId(const std::string& text) throw (InvalidProgramIdException) {
	unsigned result = parse<InvalidProgramIdException>(text);
	return static_cast<IdType>(result);
}

IdType Command::parseStartTimeId(const std::string& text) throw (InvalidStartTimeIdException) {
	unsigned result = parse<InvalidStartTimeIdException>(text);
	return static_cast<IdType>(result);
}

IdType Command::parseRunTimeId(const std::string& text) throw (InvalidRunTimeIdException) {
	unsigned result = parse<InvalidRunTimeIdException>(text);
	return static_cast<IdType>(result);
}

IdType Command::parseValveId(const std::string& text) throw (InvalidValveIdException) {
	unsigned result = parse<InvalidValveIdException>(text);
	return static_cast<IdType>(result);
}

IdType Command::parseZoneId(const std::string& text) throw (InvalidZoneIdException) {
	unsigned result = parse<InvalidZoneIdException>(text);
	return static_cast<IdType>(result);
}

unsigned Command::parseUnsigned(const std::string& text) throw (InvalidParameterException) {
	return parse<InvalidParameterException>(text);
}

bool Command::parseOnOff(const std::string& text) throw (InvalidParameterException) {
	bool result;

	if (text == "on") {
		result = true;
	} else if (text == "off") {
		result = false;
	} else {
		throw InvalidParameterException();
	}

	return result;
}

