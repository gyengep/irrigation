#include "Common.h"
#include "Commands.h"

#include <iomanip>
#include <vector>

#include "Hardware/Valves.h"
#include "Model/Application.h"
#include "Model/Application.h"
#include "Utils/Parse.h"
#include "Views/CommandLineView/CommandLineView.h"


#define CHECK_PARAMETERS(req)													\
if (parameters.size() < req) {													\
	throw TooFewArgumentsException();											\
} else if (parameters.size() > req) {											\
	throw TooMuchArgumentsException();											\
}


#define PRINT_RUNTIME(id, runTime)												\
	std::cout << "  " << id << " - "; 											\
	std::cout << std::setfill('0') << std::setw(2) << runTime;					\
	std::cout << " min" << std::endl

#define PRINT_RUNTIMES(runTimes)												\
	std::cout << "Run times:" << std::endl;										\
	for (auto it = runTimes.begin(); runTimes.end() != it; ++it) {				\
		PRINT_RUNTIME(it->first, it->second);									\
	}

#define PRINT_STARTTIME(id, startTime)											\
	std::cout << "  " << id << " - " << startTime << std::endl

#define PRINT_STARTTIMES(startTimes)											\
	std::cout << "Start times:" << std::endl;									\
	for (auto it = startTimes.begin(); startTimes.end() != it; ++it) {			\
		PRINT_STARTTIME(it->first, it->second);									\
	}


namespace CommandLine {

	void Help::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		std::cout << std::endl;

		const std::list<Command*>& commands = view->getCommandExecutor().getCommands();

		for (auto it = commands.begin(); it != commands.end(); ++it) {
			std::cout << (*it)->getCommand() << " " << (*it)->getComment() << std::endl;
		}
	}

	void StarttimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgram program = document->getPrograms().at(programId);

		PRINT_STARTTIMES(program->getStartTimes());
	}

	void StarttimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(4);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType startTimeId = Parser::parseStartTimeId(parameters[1]);
		unsigned hour = Parser::parseUnsigned(parameters[2]);
		unsigned min = Parser::parseUnsigned(parameters[3]);

		LockedProgram program = document->getPrograms().at(programId);
		program->getStartTimes().modify(startTimeId, StartTime(hour, min));
	}

	void StarttimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType startTimeId = Parser::parseStartTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);
		StartTime startTime = program->getStartTimes().at(startTimeId);

		PRINT_STARTTIME(startTimeId, startTime);
	}

	void StarttimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = Parser::parseProgramId(parameters[0]);
		unsigned hour = Parser::parseUnsigned(parameters[1]);
		unsigned min = Parser::parseUnsigned(parameters[2]);

		LockedProgram program = document->getPrograms().at(programId);
		program->getStartTimes().insert(StartTime(hour, min));
	}

	void StarttimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType startTimeId = Parser::parseStartTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);
		program->getStartTimes().erase(startTimeId);
	}

	void RuntimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgram program = document->getPrograms().at(programId);

		PRINT_RUNTIMES(program->getRunTimes());
	}

	void RuntimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType runTimeId = Parser::parseRunTimeId(parameters[1]);
		unsigned runTime = Parser::parseUnsigned(parameters[2]);

		LockedProgram program = document->getPrograms().at(programId);

		program->getRunTimes().at(runTimeId) = runTime;
	}

	void RuntimeGet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType runTimeId = Parser::parseRunTimeId(parameters[1]);

		LockedProgram program = document->getPrograms().at(programId);
		unsigned runTime = program->getRunTimes().at(runTimeId);

		PRINT_RUNTIME(runTimeId, runTime);
	}

	void ProgramList::onIterate(IdType id, LockedProgram program) {
		std::cout << id << " - " << program->getName() << std::endl;
	}

	void ProgramList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		std::cout << "Programs:" << std::endl;

		auto f = std::bind(&ProgramList::onIterate, this, std::placeholders::_1, std::placeholders::_2);
		document->getPrograms().iterate(f);
	}

	void ProgramShow::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgram program = document->getPrograms().at(programId);

		std::cout << "Name: " << std::endl;
		std::cout << "  " << program->getName() << std::endl;
		std::cout << std::endl;
		PRINT_RUNTIMES(program->getRunTimes());
		std::cout << std::endl;
		PRINT_STARTTIMES(program->getStartTimes());
	}

	void ProgramAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		document->getPrograms().insert(new Program());
	}

	void ProgramDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);

		document->getPrograms().erase(programId);
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);

		LockedProgram program = document->getPrograms().at(programId);
		program->setName(parameters[1]);
	}

	void ProgramMove::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		unsigned position = Parser::parseUnsigned(parameters[1]);

		document->getPrograms().move(programId, position);
	}

	void Valve::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType valveId = Parser::parseValveId(parameters[0]);
		bool active = Parser::parseOnOff(parameters[1]);

		Valves::getInstance().activate(valveId, active);
	}

	void Zone::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType zoneId = Parser::parseZoneId(parameters[0]);
		bool active = Parser::parseOnOff(parameters[1]);

		document->getZones().activate(zoneId, active);
	}

	void ResetValves::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		for (size_t i = 0; i < VALVE_COUNT; i++) {
			Valves::getInstance().activate(i, false);
		}
	}

	void Exit::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);
		getApplication()->terminate();
	}
}
