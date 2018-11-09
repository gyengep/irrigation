#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include <sstream>
#include "Schedulers/SpecifiedScheduler.h"
#include "Logger/Logger.h"

using namespace std;


Program::Program() :
	name(),
	schedulerType(SchedulerType::SPECIFIED_DAYS),
	specifiedScheduler(schedulerFactory.createSpecifiedScheduler()),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
}

Program::~Program() {
}

const string& Program::getName() const {
	return name;
}

void Program::setName(const string& newName) {
	name = newName;
}

void Program::setSchedulerType(SchedulerType schedulerType) {
	this->schedulerType = schedulerType;
}

SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

const Scheduler& Program::getCurrentScheduler() const {
	switch (schedulerType) {
	case SchedulerType::SPECIFIED_DAYS:
		return getSpecifiedScheduler();
	default:
		throw invalid_argument("Program::getCurrentScheduler(): unknown SchedulerType " + to_string(static_cast<unsigned>(schedulerType)));
	}
}

bool Program::isScheduled(const time_t& rawTime) const {
	tm timeinfo = *localtime(&rawTime);
	StartTime currentTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

	for (auto& startTimeAndIdPair : *startTimes) {
		const StartTime& startTime = *startTimeAndIdPair.second;
		if (startTime == currentTime) {
			return getCurrentScheduler().isDayScheduled(timeinfo);
		}
	}

	return false;
}

const SpecifiedScheduler& Program::getSpecifiedScheduler() const {
	return *specifiedScheduler.get();
}

SpecifiedScheduler& Program::getSpecifiedScheduler() {
	return *specifiedScheduler.get();
}

ProgramDTO Program::getProgramDTO() const {
	list<RunTimeDTO> runTimeDTOs;
	list<StartTimeDTO> startTimeDTOs;

	for (auto& runTimeAndIdPair : *runTimes) {
		runTimeDTOs.push_back(runTimeAndIdPair.second->getRunTimeDTO().setId(runTimeAndIdPair.first));
	}

	for (auto& startTimeAndIdPair : *startTimes) {
		startTimeDTOs.push_back(startTimeAndIdPair.second->getStartTimeDTO().setId(startTimeAndIdPair.first));
	}

	return ProgramDTO(name.c_str(), "specified",
			move(getSpecifiedScheduler().getSpecifiedSchedulerDTO()),
			move(runTimeDTOs),
			move(startTimeDTOs));
}

void Program::updateFromDTO(const ProgramDTO& programDTO) {
	if (programDTO.hasName()) {
		setName(programDTO.getName());
	}

	if (programDTO.hasSchedulerType()) {
		if (programDTO.getSchedulerType() != "specified") {
			throw invalid_argument("Program::updateFromDTO(): invalid SchedulerType: " + programDTO.getSchedulerType());
		}

		setSchedulerType(SchedulerType::SPECIFIED_DAYS);
	}

	if (programDTO.hasSpecifiedScheduler()) {
		specifiedScheduler = schedulerFactory.createSpecifiedScheduler();
		specifiedScheduler->updateFromDTO(programDTO.getSpecifiedScheduler());
	}

	if (programDTO.hasRunTimes()) {
		runTimes.reset(new RunTimeContainer());

		unsigned id = 0;
		for (const RunTimeDTO& runTimeDTO : programDTO.getRunTimes()) {
			if (runTimeDTO.hasId()) {
				id = runTimeDTO.getId();
			}

			runTimes->at(IdType(id))->updateFromDTO(runTimeDTO);
			id++;
		}
	}

	if (programDTO.hasStartTimes()) {
		startTimes.reset(new StartTimeContainer());

		for (const StartTimeDTO& startTimeDTO : programDTO.getStartTimes()) {
			unique_ptr<StartTime> startTime(new StartTime());
			startTime->updateFromDTO(startTimeDTO);

			if (startTimeDTO.hasId()) {
				startTimes->insert(IdType(startTimeDTO.getId()), move(startTime));
			} else {
				startTimes->insert(IdType(), move(startTime));
			}
		}
	}
}

string to_string(const Program& program) {
	ostringstream o;
	o << "Program{";
	o << "name=\"" << program.getName() << "\", ";
	o << "schedulerType=\"" << to_string(program.getSchedulerType()) << "\", ";
	o << "specifiedScheduler=" << to_string(program.getSpecifiedScheduler()) << ", ";
	o << "runTimes=" << to_string(program.getRunTimes()) << ", ";
	o << "startTimes=" << to_string(program.getStartTimes());
	o << "}";

	return o.str();
}
