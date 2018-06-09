#include "Program.h"
#include "Exceptions.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;



SpecifiedScheduler* SchedulerFactory::createSpecifiedSheduler() const {
	return new SpecifiedScheduler();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Program::Program(const string& name) :
	name(name),
	schedulerType(SchedulerType::SPECIFIED_DAYS),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
	initSchedulers(SchedulerFactory());
}

Program::Program(const SchedulerFactory& schedulerFactory) :
	schedulerType(SchedulerType::SPECIFIED_DAYS),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
	initSchedulers(schedulerFactory);
}

Program::~Program() {
}

void Program::initSchedulers(const SchedulerFactory& schedulerFactory) {
	specifiedScheduler.reset(schedulerFactory.createSpecifiedSheduler());
}

string Program::getName() const {
	return name;
}

void Program::setName(const string& newName) {
	name = newName;
}

void Program::setSchedulerType(SchedulerType schedulerType) {
	switch(schedulerType) {
	case SchedulerType::SPECIFIED_DAYS:
		this->schedulerType = schedulerType;
		break;
	default:
		throw InvalidSchedulerTypeException(schedulerType);
	}
}

SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

const Scheduler& Program::getCurrentScheduler() const {
	switch (schedulerType) {
	case SchedulerType::SPECIFIED_DAYS:
		return getSpecifiedScheduler();
	default:
		throw logic_error("Invalid scheduler type");
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
		runTimeDTOs->push_back(it->second->getRunTimeDTO());
	}

	for (auto it = startTimes->begin(); it != startTimes->end(); ++it) {
		startTimeDTOs->push_back(it->second->getStartTimeDTO());
	}

	return ProgramDTO(name.c_str(), "specified",
			getSpecifiedScheduler().getSpecifiedSchedulerDTO(),
			runTimeDTOs.release(),
			startTimeDTOs.release());
}
