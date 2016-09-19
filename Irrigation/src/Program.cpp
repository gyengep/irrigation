/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Program.h"

#include "Tools.h"
#include "DaySchedulerSpecified.h"


Program::Program() {
	schedulerType = SPECIFIED_DAYS;
	schedulers[SPECIFIED_DAYS] = new DayScheduler_Specified();

}

Program::~Program() {
	for (auto it = schedulers.begin(); schedulers.end() != it; ++it) {
		delete *it;
	}
}

////////////////////////////////////////////////////////////////

bool Program::isScheduled(const std::time_t& rawTime) const {
	std::tm timeinfo = *std::localtime(&rawTime);
	return schedulers[schedulerType]->isDayScheduled(&timeinfo);
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	return name;
}

void Program::setName(const std::string& newName) {
	name = newName;
}


