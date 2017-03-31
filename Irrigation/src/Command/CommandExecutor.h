#pragma once

#include <list>
#include <mutex>
#include <string>
#include <vector>


class IrrigationDocument;

typedef std::vector<std::string> Tokens;


class Command {
	const std::string command;

protected:
	IrrigationDocument* const document;

public:
	Command(IrrigationDocument* document, const char* command) : command(command), document(document) {}
	virtual ~Command() {}

	virtual void execute(const Tokens& parameters) = 0;

	const std::string& getCommand() const { return command; }
};


class CommandExecutor {
	std::list<Command*> commands;
	mutable std::mutex mtx;

	Command* getMatchingCommand(const std::string& command) const;

public:
	CommandExecutor();
	virtual ~CommandExecutor();

	void addCommand(Command* command);
	void execute(const Tokens& tokens) const;
};
