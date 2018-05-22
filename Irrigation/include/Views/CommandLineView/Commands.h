#pragma once
#include "CommandExecutor/Command.h"
#include "CommandExecutor/CommandExecutor.h"
#include "Model/IrrigationDocument.h"


class IrrigationDocument;


#define DECLARE_COMMAND(CLASS_NAME, COMMAND, PARAMETERS)			\
	class CLASS_NAME : public IrrigationCommandLine {				\
	public:															\
		CLASS_NAME(IrrigationDocument* document) :					\
		IrrigationCommandLine(document, COMMAND, PARAMETERS) {}		\
		virtual void execute(const Tokens& parameters);				\
	}



namespace CommandLine {

	class IrrigationCommandLine : public Command {
	protected:
		IrrigationDocument* document;

	public:
		IrrigationCommandLine(IrrigationDocument* document, const char* command, const char* comment) :
			Command(command, comment),
			document(document)
		{
		}
	};

	DECLARE_COMMAND(ProgramShow, "showprogram", "<ProgramId>");
	DECLARE_COMMAND(ProgramAdd, "addprogram", "name");
	DECLARE_COMMAND(ProgramDelete, "delprogram", "<ProgramId>");
	DECLARE_COMMAND(ProgramRename, "renprogram", "<ProgramId> name");
	DECLARE_COMMAND(StartTimeList, "liststarttime", "<ProgramId>");
	DECLARE_COMMAND(StartTimeAdd, "addstarttime", "<ProgramId> hour min");
	DECLARE_COMMAND(StartTimeDelete, "delstarttime", "<ProgramId> <StartTimeId>");
	DECLARE_COMMAND(StartTimeSet, "setstarttime", "<ProgramId> <StartTimeId> hour min");
	DECLARE_COMMAND(RunTimeList, "listruntime", "<ProgramId>");
	DECLARE_COMMAND(RunTimeSet, "setruntime", "<ProgramId> <RunTimeId> min");
	DECLARE_COMMAND(ResetValves, "resetvalves", "");
	DECLARE_COMMAND(Valve, "valve", "<ValveId> on|off");
	DECLARE_COMMAND(Zone, "zone", "<ZoneId> on|off");
	DECLARE_COMMAND(Exit, "exit", "");


	class Help : public IrrigationCommandLine {
		const CommandExecutor& commandExecutor;
	public:
		Help(const CommandExecutor& commandExecutor, IrrigationDocument* document) : IrrigationCommandLine(document, "help", ""), commandExecutor(commandExecutor) {}
		virtual void execute(const Tokens& parameters);
	};

	class ProgramList : public IrrigationCommandLine {
		void onIterate(const IdType&, LockedProgramPtr);
	public:
		ProgramList(IrrigationDocument* document) : IrrigationCommandLine(document, "listprogram", "") {}
		virtual void execute(const Tokens& parameters);
	};
}
