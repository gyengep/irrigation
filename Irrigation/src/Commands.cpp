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

		IdType programId = parseProgramId(parameters[0]);

		AUTO_LOCK_PROGRAMS();
		const Program& program = document->programs().get(programId);

		callback->onStarttimeListSuccess(program.startTimes());
	}

	void StarttimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);
		unsigned startTime = parseUnsigned(parameters[2]);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().set(startTimeId, startTime);

		callback->onStarttimeSetSuccess();
	}

	void StarttimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);

		AUTO_LOCK_PROGRAMS();
		unsigned startTime = document->programs().get(programId).startTimes().get(startTimeId);

		callback->onStarttimeGetSuccess(startTimeId, startTime);
	}

	void StarttimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		unsigned startTime = parseUnsigned(parameters[1]);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().add(startTime);

		callback->onStarttimeAddSuccess();
	}

	void StarttimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).startTimes().del(startTimeId);

		callback->onStarttimeDeleteSuccess();
	}

	void RuntimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = parseProgramId(parameters[0]);

		AUTO_LOCK_PROGRAMS();
		const Program& program = document->programs().get(programId);

		callback->onRuntimeListSuccess(program.runTimes());
	}

	void RuntimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = parseProgramId(parameters[0]);
		IdType runTimeId = parseRunTimeId(parameters[1]);
		unsigned runTime = parseUnsigned(parameters[2]);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).runTimes().set(runTimeId, runTime);

		callback->onRuntimeSetSuccess();
	}

	void RuntimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		IdType runTimeId = parseRunTimeId(parameters[1]);

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

		IdType programId = parseProgramId(parameters[0]);

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

		IdType programId = parseProgramId(parameters[0]);

		AUTO_LOCK_PROGRAMS();
		document->programs().del(programId);

		callback->onProgramDeleteSuccess();
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);

		AUTO_LOCK_PROGRAMS();
		document->programs().get(programId).setName(parameters[1]);

		callback->onProgramRenameSuccess();
	}

	void ProgramMove::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		unsigned position = parseUnsigned(parameters[1]);

		AUTO_LOCK_PROGRAMS();
		document->programs().move(programId, position);

		callback->onProgramMoveSuccess();
	}

	void Valve::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType valveId = parseValveId(parameters[0]);
		bool open = parseOnOff(parameters[1]);
		document->openValve(valveId, open);

		callback->onValveSuccess();
	}

	void Zone::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType zoneId = parseZoneId(parameters[0]);
		bool open = parseOnOff(parameters[1]);
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
