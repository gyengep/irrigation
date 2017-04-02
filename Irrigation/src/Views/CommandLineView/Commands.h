#pragma once

#include "Command/CommandExecutor.h"
#include "Model/IrrigationDocument.h"


class CommandLineView;

#define DECLARE_COMMAND(CLASS_NAME, COMMAND, PARAMETERS)		\
	class CLASS_NAME : public Command {							\
	public:														\
		CLASS_NAME(IrrigationDocument* document) :				\
			Command(document, COMMAND, PARAMETERS) {}			\
		virtual void execute(const Tokens& parameters);			\
	}



namespace CommandLine {

	DECLARE_COMMAND(Exit, "exit", "");
	DECLARE_COMMAND(Valve, "valve", "<ValveId> on|off");
	DECLARE_COMMAND(Zone, "zone", "<ZoneId> on|off");
	DECLARE_COMMAND(ProgramShow, "showprogram", "<ProgramId>");
	DECLARE_COMMAND(ProgramAdd, "addprogram", "");
	DECLARE_COMMAND(ProgramDelete, "delprogram", "<ProgramId>");
	DECLARE_COMMAND(ProgramRename, "renprogram", "<ProgramId> name");
	DECLARE_COMMAND(ProgramMove, "moveprogram", "<ProgramId> position");
	DECLARE_COMMAND(StarttimeList, "liststarttime", "<ProgramId>");
	DECLARE_COMMAND(StarttimeAdd, "addstarttime", "<ProgramId> hour min");
	DECLARE_COMMAND(StarttimeDelete, "delstarttime", "<ProgramId> <StartTimeId>");
	DECLARE_COMMAND(StarttimeSet, "setstarttime", "<ProgramId> <StartTimeId> hour min");
	DECLARE_COMMAND(StarttimeGet, "getstarttime", "<ProgramId> <StartTimeId>");
	DECLARE_COMMAND(RuntimeList, "listruntime", "<ProgramId>");
	DECLARE_COMMAND(RuntimeSet, "setruntime", "<ProgramId> <RunTimeId> min");
	DECLARE_COMMAND(RuntimeGet, "getruntime", "<ProgramId> <RunTimeId>");
	DECLARE_COMMAND(ResetValves, "resetvalves", "");


	class Help : public Command {
		const CommandLineView* const view;
	public:
		Help(CommandLineView* const view, IrrigationDocument* document) : Command(document, "help", ""), view(view) {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramList : public Command {
		void onIterate(IdType, LockedProgram);
	public:
		ProgramList(IrrigationDocument* document) : Command(document, "listprogram", "") {}
		virtual void execute(const Tokens& parameters);
	};

	class SpaceHolder : public Command {
	public:
		SpaceHolder() : Command(NULL, "", "") {}
		virtual void execute(const Tokens& parameters) {}
	};

}
