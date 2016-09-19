/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Program.h"

#include "Tools.h"
#include "Scheduler.h"


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

bool Program::isScheduled(const std::time_t& rawTime) const {
	std::tm timeinfo = *std::localtime(&rawTime);

// TODO starttime
//	for (auto it = startTimeContainer.begin(); startTimeContainer.end() != it; ++it) {
//		if ()
//	}

	return schedulers[schedulerType]->isScheduled(&timeinfo);
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	return name;
}

void Program::setName(const std::string& newName) {
	name = newName;
}


