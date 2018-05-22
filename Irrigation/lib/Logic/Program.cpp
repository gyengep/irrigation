#include "Program.h"
#include "Exceptions.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Schedulers/Scheduler.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;


Scheduler* SchedulerFactory::createScheduler(Program::SchedulerType schedulerType) const {
	switch (schedulerType) {
	case Program::SPECIFIED_DAYS:
		return new SpecifiedScheduler();
	default:
		throw invalid_argument("Invalid schedulerType: " + to_string(schedulerType));
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Program::Program(const string& name) :
	name(name),
	schedulerType(SPECIFIED_DAYS),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
	unique_ptr<const SchedulerFactory> ptr(new SchedulerFactory());
	initSchedulers(move(ptr));
}

Program::Program(const SchedulerFactory* schedulerFactory) :
	schedulerType(SPECIFIED_DAYS),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
	unique_ptr<const SchedulerFactory> ptr(schedulerFactory ? schedulerFactory : new SchedulerFactory());
	initSchedulers(move(ptr));
}

Program::~Program() {
}

void Program::initSchedulers(unique_ptr<const SchedulerFactory> schedulerFactory) {
	schedulers.resize(1);
	schedulers[SPECIFIED_DAYS].reset(schedulerFactory->createScheduler(SPECIFIED_DAYS));
}

string Program::getName() const {
	return name;
}

void Program::setName(const string& newName) {
	name = newName;
}


void Program::setSchedulerType(SchedulerType schedulerType) {
	if (schedulerType >= schedulers.size()) {
		throw InvalidSchedulerException();
	}

	this->schedulerType = schedulerType;
}

Program::SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

const Scheduler& Program::getCurrentScheduler() const {
	return *schedulers[schedulerType].get();
}

bool Program::isScheduled(const time_t& rawTime) const {
	tm timeinfo = *localtime(&rawTime);
	StartTime currentTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

	for (auto it = startTimes->begin(); startTimes->end() != it; ++it) {
		const StartTime& startTime = *it->second;
		if (startTime.isEnabled() && startTime == currentTime) {
			return getCurrentScheduler().isDayScheduled(timeinfo);
		}
	}

	return false;
}

const SpecifiedScheduler& Program::getSpecifiedScheduler() const {
	return *dynamic_cast<SpecifiedScheduler*>(schedulers[SPECIFIED_DAYS].get());
}

SpecifiedScheduler& Program::getSpecifiedScheduler() {
	return *dynamic_cast<SpecifiedScheduler*>(schedulers[SPECIFIED_DAYS].get());
}
