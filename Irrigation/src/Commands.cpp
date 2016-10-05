/*
 * Commands.cpp
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#include "common.h"
#include "Commands.h"

#include "Application.h"
#include "Document.h"


#define AUTO_LOCK_PROGRAMS() std::lock_guard<std::mutex> lock(document->programs().getMutex())


#define CHECK_PARAMETERS(req)				\
if (parameters.size() < req) {				\
	throw TooFewArgumentsException();		\
} else if (parameters.size() > req) {		\
	throw TooMuchArgumentsException();		\
}



namespace command {

	void Help::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);
		callback->onHelpSuccess();
	}

	void StarttimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = document->programs().get(programId);

		callback->onStarttimeListSuccess(program);
	}

	void StarttimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);
		unsigned startTime = CommandExecutor::parseUInt(parameters[2], "Invalid starttime");

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().set(startTimeId, startTime);

		callback->onStarttimeSetSuccess();
	}

	void StarttimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);

		AUTO_LOCK_PROGRAMS();
		unsigned startTime = document->programs().get(programId).startTimes().get(startTimeId);

		callback->onStarttimeGetSuccess(startTimeId, startTime);
	}

	void StarttimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		unsigned startTime = CommandExecutor::parseUInt(parameters[1], "Invalid starttime");

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().add(startTime);

		callback->onStarttimeAddSuccess();
	}

	void StarttimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().del(startTimeId);

		callback->onStarttimeDeleteSuccess();
	}

	void RuntimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = document->programs().get(programId);

		callback->onRuntimeListSuccess(program);
	}

	void RuntimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = CommandExecutor::parseId(parameters[1], INVALID_RUNTIMEID);
		unsigned runTime = CommandExecutor::parseUInt(parameters[2], "Invalid runtime");

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).runTimes().set(runTimeId, runTime);

		callback->onRuntimeSetSuccess();
	}

	void RuntimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = CommandExecutor::parseId(parameters[1], INVALID_RUNTIMEID);

		AUTO_LOCK_PROGRAMS();
		unsigned runTime = document->programs().get(programId).runTimes().get(runTimeId);

		callback->onRuntimeGetSuccess(runTimeId, runTime);
	}

	void ProgramList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		AUTO_LOCK_PROGRAMS();
		const Document::Programs& programs = document->programs().container();

		callback->onProgramListSuccess(programs);
	}

	void ProgramShow::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = document->programs().get(programId);

		callback->onProgramShowSuccess(program);
	}

	void ProgramAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		AUTO_LOCK_PROGRAMS();
		Program& program = document->programs().add();
		program.setName(parameters[0]);

		callback->onProgramAddSuccess();
	}

	void ProgramDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		document->programs().del(programId);

		callback->onProgramDeleteSuccess();
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).setName(parameters[1]);

		callback->onProgramRenameSuccess();
	}

	void ProgramMove::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		unsigned position = CommandExecutor::parseUInt(parameters[1], "Invalid position");

		AUTO_LOCK_PROGRAMS();
		document->programs().move(programId, position);

		callback->onProgramMoveSuccess();
	}

	void Valve::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType valveId = CommandExecutor::parseId(parameters[0], INVALID_VALVEID);
		bool open = CommandExecutor::parseOnOff(parameters[1], "Invalid parameter");
		document->openValve(valveId, open);

		callback->onValveSuccess();
	}

	void Zone::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType zoneId = CommandExecutor::parseId(parameters[0], INVALID_ZONEID);
		bool open = CommandExecutor::parseOnOff(parameters[1], "Invalid parameter");
		document->openZone(zoneId, open);

		callback->onZoneSuccess();
	}

	void ResetValves::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		for (unsigned i = 0; i < VALVE_COUNT; i++) {
			document->openValve(i, false);
		}

		callback->onResetValvesSuccess();
	}

	void Exit::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);
		getApplication()->terminate();

		callback->onExitSuccess();
	}
}
