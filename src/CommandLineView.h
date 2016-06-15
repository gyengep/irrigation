/*
 * CommandLineView.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef COMMANDLINEVIEW_H_
#define COMMANDLINEVIEW_H_

#include <list>
#include <vector>
#include <string>
#include "View.h"

namespace std {
	class thread;
}

class CommandLineView : public View {
public:

	typedef std::vector<std::string> Tokens;

private:

	class CommandLineException : public std::exception {
		const std::string message;
	public:
		CommandLineException(const char* message) : message(message) {}
		virtual const char* what() const throw() { return message.c_str(); }
	};

	class TooFewArgumentsException : public CommandLineException {
	public:
		TooFewArgumentsException() : CommandLineException("Too few arguments") {}
	};

	class TooMuchArgumentsException : public CommandLineException {
	public:
		TooMuchArgumentsException() : CommandLineException("Too much arguments") {}
	};

	class UnknownSubcommandException : public CommandLineException {
	public:
		UnknownSubcommandException() : CommandLineException("Unknown subcommand") {}
	};

	typedef void (*FuncType)(CommandLineView*, const std::string&, const Tokens&);

	struct Commands {
		const char* cmd;
		bool subcommand;
		FuncType func;
	};

	std::thread* workerThread;
	volatile bool isTerminated;

	static Commands commands[];

	static void workerFunc(CommandLineView* p);

	//static void cmd_exit(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	static void cmd_help(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	static void cmd_program(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	//static void cmd_runtime(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	//static void cmd_starttime(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	static void cmd_valve(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);
	static void cmd_zone(CommandLineView* p, const std::string& subcommand, const Tokens& parameters);

public:
	CommandLineView(Document* document);
	virtual ~CommandLineView();

	static unsigned parseUInt(const std::string& text, const char* errorMessage);
	static bool parseOnOff(const std::string& text, const char* errorMessage);
	static void tokenize(const std::string& text, Tokens& tokens);
};

#endif /* COMMANDLINEVIEW_H_ */
