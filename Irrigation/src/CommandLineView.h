/*
 * CommandLineView.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef COMMANDLINEVIEW_H_
#define COMMANDLINEVIEW_H_

#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "View.h"


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

	typedef void (CommandLineView::*FuncType)(const std::string&, const Tokens&);

	struct Commands {
		const char* cmd;
		bool subcommand;
		FuncType func;
	};

	std::thread* workerThread;
	std::atomic_bool isTerminated;

	static Commands commands[];

	void workerFunc();

	void cmd_exit(const std::string& subcommand, const Tokens& parameters);
	void cmd_help(const std::string& subcommand, const Tokens& parameters);
	void cmd_program(const std::string& subcommand, const Tokens& parameters);
	void cmd_runtime(const std::string& subcommand, const Tokens& parameters);
	//void cmd_startprog(const std::string& subcommand, const Tokens& parameters);
	//void cmd_startzone(const std::string& subcommand, const Tokens& parameters);
	//void cmd_stop(const std::string& subcommand, const Tokens& parameters);
	void cmd_starttime(const std::string& subcommand, const Tokens& parameters);
	void cmd_valve(const std::string& subcommand, const Tokens& parameters);
	void cmd_zone(const std::string& subcommand, const Tokens& parameters);
	void cmd_reset(const std::string& subcommand, const Tokens& parameters);

	static IdType parseId(const std::string& text, const char* errorMessage);
	static unsigned parseUInt(const std::string& text, const char* errorMessage);
	static bool parseOnOff(const std::string& text, const char* errorMessage);
	static void tokenize(const std::string& text, Tokens& tokens);

public:
	CommandLineView(Document* document);
	virtual ~CommandLineView();
};

#endif /* COMMANDLINEVIEW_H_ */
