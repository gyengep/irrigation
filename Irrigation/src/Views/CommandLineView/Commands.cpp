/*
 * Commands.cpp
 *
 *  Created on: 2016.10.05.
 *      Author: 502664609
 */

#include "Common.h"
#include "CommandExecutor.h"
#include "Commands.h"

#include <vector>

#include "Hardware/Valve.h"
#include "Model/Application.h"
#include "Model/Application.h"


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

		LockedProgram program = document->getPrograms().at(programId);

		callback->onStarttimeListSuccess(program->getStartTimes());
	}

	void StarttimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);
/*
		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);
		unsigned startTime = parseUnsigned(parameters[2]);

		LockedProgram program = document->getPrograms().at(programId);

		program->getStartTimes().at(startTimeId) = startTime;

		callback->onStarttimeSetSuccess();
*/
	}

	void StarttimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);
/*
		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);

		callback->onStarttimeGetSuccess(startTimeId, program->getStartTimes().at(startTimeId));
*/
	}

	void StarttimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);
/*
		IdType programId = parseProgramId(parameters[0]);
		unsigned startTime = parseUnsigned(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);

		program->getStartTimes().insert(startTime);

		callback->onStarttimeAddSuccess();
*/
	}

	void StarttimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		IdType startTimeId = parseStartTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);

		program->getStartTimes().erase(startTimeId);

		callback->onStarttimeDeleteSuccess();
	}

	void RuntimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = parseProgramId(parameters[0]);

		LockedProgram program = document->getPrograms().at(programId);

		callback->onRuntimeListSuccess(program->getRunTimes());
	}

	void RuntimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = parseProgramId(parameters[0]);
		IdType runTimeId = parseRunTimeId(parameters[1]);
		unsigned runTime = parseUnsigned(parameters[2]);

		LockedProgram program = document->getPrograms().at(programId);

		program->getRunTimes().at(runTimeId) = runTime;

		callback->onRuntimeSetSuccess();
	}

	void RuntimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		IdType runTimeId = parseRunTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);

		callback->onRuntimeGetSuccess(runTimeId, program->getRunTimes().at(runTimeId));
	}

	void ProgramList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		callback->onProgramListSuccess(document->getPrograms());
	}

	void ProgramShow::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = parseProgramId(parameters[0]);

		LockedProgram program = document->getPrograms().at(programId);

		callback->onProgramShowSuccess(*program);
	}

	void ProgramAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		document->getPrograms().insert(new Program());
		callback->onProgramAddSuccess();
	}

	void ProgramDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = parseProgramId(parameters[0]);

		document->getPrograms().erase(programId);
		callback->onProgramDeleteSuccess();
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);

		LockedProgram program = document->getPrograms().at(programId);

		program->setName(parameters[1]);

		callback->onProgramRenameSuccess();
	}

	void ProgramMove::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = parseProgramId(parameters[0]);
		unsigned position = parseUnsigned(parameters[1]);

		document->getPrograms().move(programId, position);
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
