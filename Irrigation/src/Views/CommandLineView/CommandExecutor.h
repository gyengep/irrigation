/*
 * CommandExecutor.h
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include <climits>
#include <list>
#include <mutex>
#include <vector>

#include "Model/Document.h"

class Program;


typedef std::vector<std::string> Tokens;



class CommandCallback {
public:
	virtual ~CommandCallback() {}

	virtual void onHelpSuccess() = 0;
	virtual void onStarttimeListSuccess(const StartTimeContainer& startTimes) = 0;
	virtual void onStarttimeSetSuccess() = 0;
	virtual void onStarttimeGetSuccess(IdType startTimeId, unsigned startTime) = 0;
	virtual void onStarttimeAddSuccess() = 0;
	virtual void onStarttimeDeleteSuccess() = 0;
	virtual void onRuntimeListSuccess(const RunTimeContainer& runTimes) = 0;
	virtual void onRuntimeSetSuccess() = 0;
	virtual void onRuntimeGetSuccess(IdType runTimeId, unsigned runTime) = 0;
	virtual void onProgramListSuccess(const ProgramContainer& programs) = 0;
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

	template<class T> static unsigned parse(const std::string& text) {
		std::size_t pos;
		unsigned long result;
		try {
			result = std::stoul(text, &pos, 10);
		} catch (std::invalid_argument& e) {
			throw T();
		}

		if (text.length() != pos || result > UINT_MAX) {
			throw T();
		}
		return result;
	}

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

	static IdType parseProgramId(const std::string& text) throw (InvalidProgramIdException);
	static IdType parseRunTimeId(const std::string& text) throw (InvalidRunTimeIdException);
	static IdType parseStartTimeId(const std::string& text) throw (InvalidStartTimeIdException);
	static IdType parseValveId(const std::string& text) throw (InvalidValveIdException);
	static IdType parseZoneId(const std::string& text) throw (InvalidZoneIdException);
	static unsigned parseUnsigned(const std::string& text) throw (InvalidParameterException);
	static bool parseOnOff(const std::string& text) throw (InvalidParameterException);
};


class CommandExecutor {
	std::list<Command*> commands;
	mutable std::mutex mtx;

	Command& getMatchingCommand(Tokens& tokens) const;

public:
	CommandExecutor();
	virtual ~CommandExecutor();

	void addCommand(Command* command);
	void execute(const Tokens& tokens) const throw (CommandException);
};

#endif /* COMMANDEXECUTOR_H_ */
