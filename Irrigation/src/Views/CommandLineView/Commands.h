/*
 * Commands.h
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "CommandExecutor.h"


#define DECLARE_COMMAND(CLASS_NAME, COMMAND, SUBCOMMAND)		\
	class CLASS_NAME : public Command {							\
	public:														\
		CLASS_NAME(CommandCallback* callback, Document* doc) :	\
			Command(callback, doc, COMMAND, SUBCOMMAND) {}		\
		virtual void execute(const Tokens& parameters);			\
	};


namespace command {

	DECLARE_COMMAND(Help, "help", NULL);
	DECLARE_COMMAND(Exit, "exit", NULL);
	DECLARE_COMMAND(Valve, "valve", NULL);
	DECLARE_COMMAND(Zone, "zone", NULL);
	DECLARE_COMMAND(ProgramList, "program", "list");
	DECLARE_COMMAND(ProgramShow, "program", "show");
	DECLARE_COMMAND(ProgramAdd, "program", "add");
	DECLARE_COMMAND(ProgramDelete, "program", "delete");
	DECLARE_COMMAND(ProgramRename, "program", "rename");
	DECLARE_COMMAND(ProgramMove, "program", "move");
	DECLARE_COMMAND(StarttimeList, "starttime", "list");
	DECLARE_COMMAND(StarttimeAdd, "starttime", "add");
	DECLARE_COMMAND(StarttimeDelete, "starttime", "delete");
	DECLARE_COMMAND(StarttimeSet, "starttime", "set");
	DECLARE_COMMAND(StarttimeGet, "starttime", "get");
	DECLARE_COMMAND(RuntimeList, "runtime", "list");
	DECLARE_COMMAND(RuntimeSet, "runtime", "set");
	DECLARE_COMMAND(RuntimeGet, "runtime", "get");
	DECLARE_COMMAND(ResetValves, "reset", "valves");

}

#endif /* COMMANDS_H_ */
