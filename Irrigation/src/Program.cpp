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


Program::Program() :
	nextStartTimeId(0)
{
	schedulers[SPECIFIED_DAYS] = new DayScheduler_Specified();

	for (IdType i = 0; i < runTimes.size(); i++) {
		runTimes[i] = std::make_pair(i, 0);
	}
}

Program::~Program() {
	for (auto it = schedulers.begin(); schedulers.end() != it; ++it) {
		delete *it;
	}
}

////////////////////////////////////////////////////////////////

bool Program::isScheduled(std::time_t rawTime) const {
	std::tm* timeinfo = std::localtime(&rawTime);
	return schedulers[schedulerType]->isDayScheduled(timeinfo);
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	return name;
}

void Program::setName(const std::string& newName) {
	name = newName;
}

////////////////////////////////////////////////////////////////
// RUN TIME

const Program::RunTimes& Program::getRunTimes() const {
	return runTimes;
}

void Program::setRunTime(IdType id, unsigned minutes) {
	try {
		tools::set(runTimes, id, minutes);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_RUNTIMEID);
	}
}

unsigned Program::getRunTime(IdType id) const {
	unsigned result;

	try {
		result = tools::get(runTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_RUNTIMEID);
	}

	return result;
}

////////////////////////////////////////////////////////////////
// START TIME

const Program::StartTimes& Program::getStartTimes() const {
	return startTimes;
}

IdType Program::addStartTime(unsigned minutes) {
	IdType startTimeId = nextStartTimeId++;
	tools::push_back(startTimes, startTimeId, minutes);
	return startTimeId;
}

void Program::deleteStartTime(IdType id) {
	try {
		tools::erase(startTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}
}

void Program::setStartTime(IdType id, unsigned minutes) {
	try {
		tools::set(startTimes, id, minutes);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}
}

unsigned Program::getStartTime(IdType id) const {
	unsigned result;

	try {
		result = tools::get(startTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}

	return result;
}
