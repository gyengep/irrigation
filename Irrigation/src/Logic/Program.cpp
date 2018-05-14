#include "Program.h"
#include "Exceptions.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Schedulers/Scheduler.h"
#include "Schedulers/SpecifiedScheduler.h"



Program::Program() :
	name(),
	schedulerType(SPECIFIED_DAYS),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
	schedulers.resize(1);
	schedulers[SPECIFIED_DAYS].reset(new SpecifiedScheduler());
}

Program::~Program() {
}

////////////////////////////////////////////////////////////////

std::string Program::getName() const {
	return name;
}

void Program::setName(const std::string& newName) {
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

Scheduler& Program::getCurrentScheduler() const {
	return *schedulers[schedulerType].get();
}

bool Program::isScheduled(const std::time_t& rawTime) const {
	std::tm timeinfo = *std::localtime(&rawTime);
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
