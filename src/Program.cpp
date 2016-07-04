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


Program::Program(SchedulerCallBack* callBack) :
	callBack(callBack),
	nextStartTimeID(0)
{
	watering[SPECIFIED] = new DayScheduler_Specified();

	for (IdType i = 0; i < runTimes.size(); i++) {
		runTimes[i] = std::make_pair(i, 0);
	}
}

Program::~Program() {
	std::lock_guard<std::mutex> guard(wateringMutex);

	for (auto it = watering.begin(); watering.end() != it; ++it) {
		delete *it;
	}
}

////////////////////////////////////////////////////////////////

bool Program::isDayScheduled() const {
	std::time_t rawtime = std::time(NULL);
	std::tm* timeinfo = std::localtime(&rawtime);

	return watering[wateringType]->isDayScheduled(timeinfo);

}

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

IdType Program::addStartTime(unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	IdType startTimeID = nextStartTimeID++;
	tools::push_back(startTimes, startTimeID, minutes);
	nextStartTimeID++;
	return startTimeID;
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
