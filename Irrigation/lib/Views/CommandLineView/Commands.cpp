#include "Commands.h"
#include "CommandLineView.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>

#include "Hardware/Valves.h"
//#include "Model/Application.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Utils/Parse.h"
#include "Utils/UniqueID.h"


#define CHECK_PARAMETERS(req)													\
if (parameters.size() != req) {													\
	throw std::runtime_error("Invalid parameter count");						\
}

#define PRINT_RUNTIMES(runTimes)												\
	std::cout << "Run times (min):" << std::endl;								\
	for (auto it = runTimes.begin(); runTimes.end() != it; ++it) {				\
		std::cout << "  " << it->first << " - "; 								\
		std::cout << std::setfill('0') << std::setw(2) << it->second;			\
		std::cout << std::endl;													\
	}

#define PRINT_STARTTIMES(startTimes)											\
	std::cout << "Start times:" << std::endl;									\
	for (auto it = startTimes.begin(); startTimes.end() != it; ++it) {			\
		std::cout << "  " << it->first << " - " << it->second << std::endl;		\
	}


namespace CommandLine {

	void Help::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		const CommandExecutor::Commands& commands = commandExecutor.getCommands();

		std::cout << std::endl;
		for (auto it = commands.begin(); it != commands.end(); ++it) {
			std::cout << (*it)->getCommand() << " " << (*it)->getComment() << std::endl;
		}
	}

	void StartTimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);

		PRINT_STARTTIMES(lockedProgram->get()->getStartTimes());
	}

	void StartTimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(4);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType startTimeId = Parser::parseStartTimeId(parameters[1]);
		unsigned hour = Parser::parseUnsigned(parameters[2]);
		unsigned min = Parser::parseUnsigned(parameters[3]);

		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);
		lockedProgram->get()->getStartTimes().at(startTimeId)->setValue(hour, min);
	}

	void StartTimeAdd::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = Parser::parseProgramId(parameters[0]);
		unsigned hour = Parser::parseUnsigned(parameters[1]);
		unsigned min = Parser::parseUnsigned(parameters[2]);

		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);
		lockedProgram->get()->getStartTimes().insert(UniqueID::getInstance().getNextId(), new StartTime(hour, min, 0));
	}

	void StartTimeDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType startTimeId = Parser::parseStartTimeId(parameters[1]);

		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);
		lockedProgram->get()->getStartTimes().erase(startTimeId);
	}

	void RunTimeList::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);

		PRINT_RUNTIMES(lockedProgram->get()->getRunTimes());
	}

	void RunTimeSet::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(3);

		IdType programId = Parser::parseProgramId(parameters[0]);
		IdType runTimeId = Parser::parseRunTimeId(parameters[1]);
		unsigned runTime = Parser::parseUnsigned(parameters[2]);

		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);

		lockedProgram->get()->getRunTimes().at(runTimeId)->setValue(runTime);
	}

	void ProgramList::onIterate(const IdType& id, LockedProgramPtr lockedProgram) {
		std::cout << id << " - " << lockedProgram->get()->getName() << std::endl;
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
		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);

		std::cout << "Name: " << std::endl;
		std::cout << "  " << lockedProgram->get()->getName() << std::endl;
		std::cout << std::endl;
		PRINT_RUNTIMES(lockedProgram->get()->getRunTimes());
		std::cout << std::endl;
		PRINT_STARTTIMES(lockedProgram->get()->getStartTimes());
	}

	void ProgramAdd::execute(const Tokens& parameters) {
		//CHECK_PARAMETERS(1);

		//document->getPrograms().insert(UniqueID::getInstance().getNextId(), new Program(parameters[0]));
	}

	void ProgramDelete::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		IdType programId = Parser::parseProgramId(parameters[0]);

		document->getPrograms().erase(programId);
	}

	void ProgramRename::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType programId = Parser::parseProgramId(parameters[0]);
		LockedProgramPtr lockedProgram = document->getPrograms().at(programId);

		lockedProgram->get()->setName(parameters[1]);
	}

	void Valve::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(2);

		IdType valveId = Parser::parseValveId(parameters[0]);
		bool active = Parser::parseOnOff(parameters[1]);

		Valves::getInstance().activate(valveId, active);
	}

	void Zone::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(1);

		//IdType zoneId = Parser::parseZoneId(parameters[0]);
	}

	void ResetValves::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);

		for (size_t i = 0; i < VALVE_COUNT; i++) {
			Valves::getInstance().activate(i, false);
		}
	}

	void Exit::execute(const Tokens& parameters) {
		CHECK_PARAMETERS(0);
		//IrrigationApplication::getInstance().terminate();
	}
}
