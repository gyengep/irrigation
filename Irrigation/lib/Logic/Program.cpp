#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"
#include "Logger/Logger.h"
#include <sstream>

using namespace std;



Program::Program(const string& name) :
	schedulerFactory(new SchedulerFactory()),
	name(name),
	schedulerType(SchedulerType::SPECIFIED_DAYS),
	specifiedScheduler(schedulerFactory->createSpecifiedScheduler()),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
}

Program::Program(const SchedulerFactory* schedulerFactory) :
	schedulerFactory(schedulerFactory),
	name(),
	schedulerType(SchedulerType::SPECIFIED_DAYS),
	specifiedScheduler(schedulerFactory->createSpecifiedScheduler()),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
}

Program::~Program() {
}

string Program::getName() const {
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

	for (auto it = startTimes->begin(); startTimes->end() != it; ++it) {
		const StartTime& startTime = *it->second;
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
	unique_ptr<list<RunTimeDTO>> runTimeDTOs(new list<RunTimeDTO>());
	unique_ptr<list<StartTimeDTO>> startTimeDTOs(new list<StartTimeDTO>());

	for (auto it = runTimes->begin(); it != runTimes->end(); ++it) {
		runTimeDTOs->push_back(it->second->getRunTimeDTO().setId(it->first));
	}

	for (auto it = startTimes->begin(); it != startTimes->end(); ++it) {
		startTimeDTOs->push_back(it->second->getStartTimeDTO().setId(it->first));
	}

	return ProgramDTO(name.c_str(), "specified",
			move(getSpecifiedScheduler().getSpecifiedSchedulerDTO()),
			runTimeDTOs.release(),
			startTimeDTOs.release());
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
		specifiedScheduler.reset(schedulerFactory->createSpecifiedScheduler());
		specifiedScheduler->updateFromDTO(programDTO.getSpecifiedScheduler());
	}

	if (programDTO.hasRunTimes()) {
		runTimes.reset(new RunTimeContainer());

		for (const RunTimeDTO& runTimeDTO : programDTO.getRunTimes()) {
			if (!runTimeDTO.hasId()) {
				throw logic_error("Program::updateFromDTO(): !runTimeDTO.hasId()");
			}

			runTimes->at(runTimeDTO.getId())->updateFromDTO(runTimeDTO);
		}
	}

	if (programDTO.hasStartTimes()) {
		startTimes.reset(new StartTimeContainer());

		for (const StartTimeDTO& startTimeDTO : programDTO.getStartTimes()) {
			if (!startTimeDTO.hasId()) {
				throw logic_error("Program::updateFromDTO(): !startTimeDTO.hasId()");
			}

			StartTimeContainer::value_type& idAndStartTimePair = startTimes->insert(startTimeDTO.getId(), new StartTime());
			idAndStartTimePair.second->updateFromDTO(startTimeDTO);
		}
	}
}

std::string to_string(const Program& program) {
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
