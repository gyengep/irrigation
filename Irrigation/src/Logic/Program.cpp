/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include "Commons/common.h"
#include "Logic/Program.h"
#include "Logic/Scheduler.h"
#include "Utils/Tools.h"


Program::Program() {
	schedulerType = SPECIFIED_DAYS;
	schedulers[SPECIFIED_DAYS] = new SpecifiedScheduler();
}

Program::~Program() {
	for (auto it = schedulers.begin(); schedulers.end() != it; ++it) {
		delete *it;
	}
}

////////////////////////////////////////////////////////////////

void Program::setSchedulerType(SchedulerType schedulerType) {
	if (schedulerType >= LAST) {
		throw InvalidSchedulerException();
	}

	this->schedulerType = schedulerType;
}

Program::SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

bool Program::isScheduled(const std::time_t& rawTime) const {
	std::tm timeinfo = *std::localtime(&rawTime);

	if (0 == timeinfo.tm_sec && schedulers[schedulerType]->isScheduled(timeinfo)) {
		unsigned startTime = StartTimeContainer::hourMin2StartTime(timeinfo.tm_hour, timeinfo.tm_min);

		for (auto it = startTimeContainer.container().begin(); startTimeContainer.container().end() != it; ++it) {
			if (it->second == startTime) {
				return true;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	return name;
}

void Program::setName(const std::string& newName) {
	name = newName;
}


const SpecifiedScheduler& Program::getSpecifiedScheduler() const {
	return *static_cast<SpecifiedScheduler*>(schedulers[SPECIFIED_DAYS]);
}

SpecifiedScheduler& Program::getSpecifiedScheduler() {
	return *static_cast<SpecifiedScheduler*>(schedulers[SPECIFIED_DAYS]);
}
