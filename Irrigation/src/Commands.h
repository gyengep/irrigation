/*
 * Commands.h
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "CommandExecutor.h"


class TooFewArgumentsException : public CommandLineException {
public:
	TooFewArgumentsException() : CommandLineException("Too few arguments") {}
};

class TooMuchArgumentsException : public CommandLineException {
public:
	TooMuchArgumentsException() : CommandLineException("Too much arguments") {}
};


namespace command {

	class Help : public Command {
	public:
		Help(Document* doc) : Command(doc, "help") {}
		virtual void execute(const Tokens& parameters);
	};

	class Exit : public Command {
	public:
		Exit(Document* doc) : Command(doc, "exit") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramList : public Command {
	public:
		ProgramList(Document* doc) : Command(doc, "program", "list") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramShow : public Command {
	public:
		ProgramShow(Document* doc) : Command(doc, "program", "show") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramAdd : public Command {
	public:
		ProgramAdd(Document* doc) : Command(doc, "program", "add") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramDelete : public Command {
	public:
		ProgramDelete(Document* doc) : Command(doc, "program", "delete") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramRename : public Command {
	public:
		ProgramRename(Document* doc) : Command(doc, "program", "rename") {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramMove : public Command {
	public:
		ProgramMove(Document* doc) : Command(doc, "program", "move") {}
		virtual void execute(const Tokens& parameters);
	};

	class StarttimeList : public Command {
	public:
		StarttimeList(Document* doc) : Command(doc, "starttime", "list") {}
		virtual void execute(const Tokens& parameters);
	};

	class StarttimeAdd : public Command {
	public:
		StarttimeAdd(Document* doc) : Command(doc, "starttime", "add") {}
		virtual void execute(const Tokens& parameters);
	};

	class StarttimeDelete : public Command {
	public:
		StarttimeDelete(Document* doc) : Command(doc, "starttime", "d") {}
		virtual void execute(const Tokens& parameters);
	};

	class StarttimeSet : public Command {
	public:
		StarttimeSet(Document* doc) : Command(doc, "starttime", "set") {}
		virtual void execute(const Tokens& parameters);
	};

	class StarttimeGet : public Command {
	public:
		StarttimeGet(Document* doc) : Command(doc, "starttime", "get") {}
		virtual void execute(const Tokens& parameters);
	};

	class RuntimeList : public Command {
	public:
		RuntimeList(Document* doc) : Command(doc, "runtime", "list") {}
		virtual void execute(const Tokens& parameters);
	};

	class RuntimeSet : public Command {
	public:
		RuntimeSet(Document* doc) : Command(doc, "runtime", "set") {}
		virtual void execute(const Tokens& parameters);
	};

	class RuntimeGet : public Command {
	public:
		RuntimeGet(Document* doc) : Command(doc, "runtime", "get") {}
		virtual void execute(const Tokens& parameters);
	};

	class Valve : public Command {
	public:
		Valve(Document* doc) : Command(doc, "valve") {}
		virtual void execute(const Tokens& parameters);
	};

	class Zone : public Command {
	public:
		Zone(Document* doc) : Command(doc, "zone") {}
		virtual void execute(const Tokens& parameters);
	};

	class ResetValves : public Command {
	public:
		ResetValves(Document* doc) : Command(doc, "reset", "valves") {}
		virtual void execute(const Tokens& parameters);
	};

}

#endif /* COMMANDS_H_ */
