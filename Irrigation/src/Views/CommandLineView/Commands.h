#pragma once

#include "Command/CommandExecutor.h"
#include "Model/IrrigationDocument.h"


#define DECLARE_COMMAND(CLASS_NAME, COMMAND)					\
	class CLASS_NAME : public Command {				\
	public:														\
		CLASS_NAME(IrrigationDocument* document) :				\
			Command(document, COMMAND) {}				\
		virtual void execute(const Tokens& parameters);			\
	}



namespace CommandLine {

	DECLARE_COMMAND(Help, "help");
	DECLARE_COMMAND(Exit, "exit");
	DECLARE_COMMAND(Valve, "valve");
	DECLARE_COMMAND(Zone, "zone");
	DECLARE_COMMAND(ProgramShow, "program_show");
	DECLARE_COMMAND(ProgramAdd, "program_add");
	DECLARE_COMMAND(ProgramDelete, "program_delete");
	DECLARE_COMMAND(ProgramRename, "program_rename");
	DECLARE_COMMAND(ProgramMove, "program_move");
	DECLARE_COMMAND(StarttimeList, "starttime_list");
	DECLARE_COMMAND(StarttimeAdd, "starttime_add");
	DECLARE_COMMAND(StarttimeDelete, "starttime_delete");
	DECLARE_COMMAND(StarttimeSet, "starttime_set");
	DECLARE_COMMAND(StarttimeGet, "starttime_get");
	DECLARE_COMMAND(RuntimeList, "runtime_list");
	DECLARE_COMMAND(RuntimeSet, "runtime_set");
	DECLARE_COMMAND(RuntimeGet, "runtime_get");
	DECLARE_COMMAND(ResetValves, "reset_valves");


	class ProgramList : public Command {
		void onIterate(IdType, LockedProgram);
	public:
		ProgramList(IrrigationDocument* document) : Command(document, "program_list") {}
		virtual void execute(const Tokens& parameters);
	};
}
