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


#define AUTO_LOCK_PROGRAMS() std::lock_guard<std::mutex> lock(getDocument()->programs().getMutex())

#define	CHECK_SUBCOMMAND()					\
if (subcommand.empty()) { 					\
	throw SubCommandMissingException(); 	\
}

#define CHECK_PARAMETERS(req)				\
if (parameters.size() < req) {				\
	throw TooFewArgumentsException();		\
} else if (parameters.size() > req) {		\
	throw TooMuchArgumentsException();		\
}


#define PRINT_RUNTIME(id, runTime)												\
	std::cout << id << " - " << runTime << " min" << std::endl;

#define PRINT_RUNTIMES(program)													\
{																				\
	const Program::RunTimes& runTimes = (program).runTimes().container();		\
	std::cout << "Run times:" << std::endl;										\
	for (auto it = runTimes.cbegin(); runTimes.cend() != it; ++it) {			\
		PRINT_RUNTIME(it->first, it->second);									\
	}																			\
}

#define PRINT_STARTTIME(id, startTime)											\
	std::cout << id << " - " << startTime << " min" << std::endl;

#define PRINT_STARTTIMES(program)												\
{																				\
	const Program::StartTimes& startTimes = (program).startTimes().container();	\
	std::cout << "Start times:" << std::endl;									\
	for (auto it = startTimes.cbegin(); startTimes.cend() != it; ++it) {		\
		PRINT_STARTTIME(it->first, it->second);									\
	}																			\
}


namespace command {

	void Help::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		std::cout << "exit" << std::endl;
		std::cout << "help" << std::endl;

		std::cout << "program list" << std::endl;
		std::cout << "program show <programID>" << std::endl;
		std::cout << "program add <name>" << std::endl;
		std::cout << "program delete <programID>" << std::endl;
		std::cout << "program rename <programID> <name>" << std::endl;
		std::cout << "program move <programID> <newPosition>" << std::endl;

		std::cout << "reset valves" << std::endl;

		std::cout << "runtime list <programID>" << std::endl;
		std::cout << "runtime set <programID> <runtimeID> <runtime>" << std::endl;
		std::cout << "runtime get <programID> <runtimeID>" << std::endl;

		std::cout << "startime list <programID>" << std::endl;
		std::cout << "startime add <programID> <starttime>" << std::endl;
		std::cout << "startime delete <programID> <starttimeID>" << std::endl;
		std::cout << "startime set <programID> <starttimeID> <starttime>" << std::endl;
		std::cout << "startime get <programID> <starttimeID>" << std::endl;

		std::cout << "valve <valveID> {on|off}" << std::endl;
		std::cout << "zone <zoneID> {on|off}" << std::endl;

		std::cout << "startprog <programID>" << std::endl;
		std::cout << "startzone <programID> <zoneID>" << std::endl;
		std::cout << "stop" << std::endl;

		std::cout << "time get" << std::endl;
		std::cout << "time set" << std::endl;
	}

	void StarttimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = getDocument()->programs().get(programId);

		PRINT_STARTTIMES(program);
	}

	void StarttimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);
		unsigned startTime = CommandExecutor::parseUInt(parameters[2], "Invalid starttime");

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().get(programId).startTimes().set(startTimeId, startTime);
	}

	void StarttimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);

		AUTO_LOCK_PROGRAMS();
		unsigned startTime = getDocument()->programs().get(programId).startTimes().get(startTimeId);

		PRINT_STARTTIME(startTimeId, startTime);
	}

	void StarttimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		unsigned startTime = CommandExecutor::parseUInt(parameters[1], "Invalid starttime");

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().get(programId).startTimes().add(startTime);

	}

	void StarttimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = CommandExecutor::parseId(parameters[1], INVALID_STARTTIMEID);

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().get(programId).startTimes().del(startTimeId);
	}

	void RuntimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = getDocument()->programs().get(programId);

		PRINT_RUNTIMES(program);
	}

	void RuntimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = CommandExecutor::parseId(parameters[1], INVALID_RUNTIMEID);
		unsigned runTime = CommandExecutor::parseUInt(parameters[2], "Invalid runtime");

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().get(programId).runTimes().set(runTimeId, runTime);
	}

	void RuntimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = CommandExecutor::parseId(parameters[1], INVALID_RUNTIMEID);

		AUTO_LOCK_PROGRAMS();
		unsigned runTime = getDocument()->programs().get(programId).runTimes().get(runTimeId);

		PRINT_RUNTIME(runTimeId, runTime);
	}

	void ProgramList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		AUTO_LOCK_PROGRAMS();
		const Document::Programs& programs = getDocument()->programs().container();

		std::cout << "Programs:" << std::endl;
		for (auto it = programs.cbegin(); programs.cend() != it; ++it) {
			IdType id = it->first;
			const Program* program = it->second;
			std::cout << id << " - " << program->getName() << std::endl;
		}
	}

	void ProgramShow::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		const Program& program = getDocument()->programs().get(programId);

		std::cout << "Name: " << program.getName() << std::endl;

		PRINT_RUNTIMES(program);
		PRINT_STARTTIMES(program);
	}

	void ProgramAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		AUTO_LOCK_PROGRAMS();
		Program& program = getDocument()->programs().add();
		program.setName(parameters[0]);
	}

	void ProgramDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().del(programId);
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().get(programId).setName(parameters[1]);
	}

	void ProgramMove::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = CommandExecutor::parseId(parameters[0], INVALID_PROGRAMID);
		unsigned position = CommandExecutor::parseUInt(parameters[1], "Invalid position");

		AUTO_LOCK_PROGRAMS();
		getDocument()->programs().move(programId, position);
	}

	void Valve::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType valveId = CommandExecutor::parseId(parameters[0], INVALID_VALVEID);
		bool open = CommandExecutor::parseOnOff(parameters[1], "Invalid parameter");
		getDocument()->openValve(valveId, open);
	}

	void Zone::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType zoneId = CommandExecutor::parseId(parameters[0], INVALID_ZONEID);
		bool open = CommandExecutor::parseOnOff(parameters[1], "Invalid parameter");
		getDocument()->openZone(zoneId, open);
	}

	void ResetValves::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		for (unsigned i = 0; i < VALVE_COUNT; i++) {
			getDocument()->openValve(i, false);
		}
	}

	void Exit::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);
		getApplication()->terminate();
	}
}
