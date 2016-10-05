/*
 * CommandExecutor.h
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include <list>
#include <mutex>
#include <vector>

#include "Document.h"

class Document;
class Program;


typedef std::vector<std::string> Tokens;

class CommandLineException : public std::exception {
protected:
	std::string message;

	CommandLineException() : message() {}
public:
	CommandLineException(const char* message) : message(message) {}
	virtual const char* what() const throw() { return message.c_str(); }
};

class UnknownCommandException : public CommandLineException {
public:
	UnknownCommandException(const std::string& command) {
		message = "Unknown command - '" + command + "'";
	}
};

class UnknownSubcommandException : public CommandLineException {
public:
	UnknownSubcommandException() : CommandLineException("Unknown subcommand") {}
};

class CommandCallback {
public:
	virtual ~CommandCallback() {}

	virtual void onHelpSuccess() = 0;
	virtual void onStarttimeListSuccess(const Program& program) = 0;
	virtual void onStarttimeSetSuccess() = 0;
	virtual void onStarttimeGetSuccess(IdType startTimeId, unsigned startTime) = 0;
	virtual void onStarttimeAddSuccess() = 0;
	virtual void onStarttimeDeleteSuccess() = 0;
	virtual void onRuntimeListSuccess(const Program& program) = 0;
	virtual void onRuntimeSetSuccess() = 0;
	virtual void onRuntimeGetSuccess(IdType runTimeId, unsigned runTime) = 0;
	virtual void onProgramListSuccess(const Document::Programs& programs) = 0;
	virtual void onProgramShowSuccess(const Program& program) = 0;
	virtual void onProgramAddSuccess() = 0;
	virtual void onProgramDeleteSuccess() = 0;
	virtual void onProgramRenameSuccess() = 0;
	virtual void onProgramMoveSuccess() = 0;
	virtual void onValveSuccess() = 0;
	virtual void onZoneSuccess() = 0;
	virtual void onResetValvesSuccess() = 0;
	virtual void onExitSuccess() = 0;
};


class Command {
protected:
	CommandCallback* const callback;
	Document* const document;
	const std::string command;
	const std::string subCommand;

public:
	Command(CommandCallback* callback, Document* document, const char* command, const char* subCommand = NULL) :
		callback(callback),
		document(document),
		command(command),
		subCommand(subCommand ? subCommand : "")
	{
	}

	virtual ~Command() {}
	virtual void execute(const Tokens& parameters) = 0;

	const std::string& getCommand() const { return command; }
	const std::string& getSubCommand() const { return subCommand; }
};


class CommandExecutor {
	std::list<Command*> commands;
	std::mutex mtx;

	Command& getCommand(Tokens& tokens);

public:
	CommandExecutor();
	virtual ~CommandExecutor();

	void addCommand(Command* command);
	void execute(Tokens& tokens);

	virtual void onExecutionFailed(const CommandLineException& e) = 0;
	virtual void onError(const std::exception& e) = 0;

	static IdType parseId(const std::string& text, const char* errorMessage);
	static unsigned parseUInt(const std::string& text, const char* errorMessage);
	static bool parseOnOff(const std::string& text, const char* errorMessage);
};

#endif /* COMMANDEXECUTOR_H_ */
