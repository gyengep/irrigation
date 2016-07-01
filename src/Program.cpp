/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Program.h"
#include "Tools.h"

#include <stdexcept>

unsigned Program::nextProgramID = 0;
unsigned Program::nextStartTimeID = 0;


Program::Program(SchedulerCallBack* callBack) :
	callBack(callBack)
{
	for (IdType i = 0; i < days.size(); i++) {
		days[i] = std::make_pair(i, false);
	}

	for (IdType i = 0; i < runTimes.size(); i++) {
		runTimes[i] = std::make_pair(i, 0);
	}
}

Program::~Program() {
	// TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////

void Program::start(time_t rawTime) {
	/*
	std::lock_guard<std::mutex> guard(wateringStartMutex);
	wateringStart = rawTime;

	if (NULL != callBack) {
		callBack->onProgramStart();
	}
	*/
}

void Program::stop() {
	/*
	std::lock_guard<std::mutex> guard(wateringStartMutex);
	wateringStart = 0;

	if (NULL != callBack) {
		if (wateringZone < ZONE_COUNT) {
			callBack->onZoneChanged(wateringZone, false);
			wateringZone = INVALID_ZONE;
		}

		callBack->onProgramStop();
	}
	*/
}



////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	std::lock_guard<std::mutex> guard(nameMutex);
	return name;
}

void Program::setName(const std::string& newName) {
	std::lock_guard<std::mutex> guard(nameMutex);
	name = newName;
}

////////////////////////////////////////////////////////////////
// DAY

void Program::enableDay(IdType id, bool enable) {
	std::lock_guard<std::mutex> guard(dayMutex);

	try {
		tools::set(days, id, enable);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}
}

bool Program::isDayEnabled(IdType id) const {
	std::lock_guard<std::mutex> guard(dayMutex);

	bool result;
	try {
		result = tools::get(days, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}

	return result;
}

////////////////////////////////////////////////////////////////
// RUN TIME

void Program::setRunTime(IdType id, unsigned minutes) {
	std::lock_guard<std::mutex> guard(runTimeMutex);

	try {
		tools::set(runTimes, id, minutes);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_RUNTIMEID);
	}
}

unsigned Program::getRunTime(IdType id) const {
	std::lock_guard<std::mutex> guard(runTimeMutex);

	unsigned result;
	try {
		result = tools::get(runTimes, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_RUNTIMEID);
	}

	return result;
}

////////////////////////////////////////////////////////////////
// START TIME

unsigned Program::addStartTime(unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	IdType startTime = nextStartTimeID++;
	tools::push_back(startTimes, startTime, minutes);
	return startTime;
}

void Program::deleteStartTime(IdType id) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	try {
		tools::erase(startTimes, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_STARTTIMEID);
	}
}

void Program::setStartTime(IdType id, unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	try {
		tools::set(startTimes, id, minutes);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_STARTTIMEID);
	}
}

unsigned Program::getStartTime(IdType id) const {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	unsigned result;
	try {
		result = tools::get(startTimes, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_STARTTIMEID);
	}

	return result;
}
