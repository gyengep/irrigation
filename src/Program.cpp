/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include <stdexcept>

#include "Program.h"

unsigned Program::nextProgramID = 0;
unsigned Program::nextStartTimeID = 0;


Program::Program(SchedulerCallBack* callBack) :
	callBack(callBack),
	wateringStart(0),
	wateringZone(INVALID_ZONE),
	id(nextProgramID++)
{
	days.object.fill(false);
	runTimes.fill(0);
}

Program::~Program() {
	// TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////

void Program::start(time_t rawTime) {
	std::lock_guard<std::mutex> guard(wateringStartMutex);
	wateringStart = rawTime;

	if (NULL != callBack) {
		callBack->onProgramStart();
	}
}

void Program::stop() {
	std::lock_guard<std::mutex> guard(wateringStartMutex);
	wateringStart = 0;

	if (NULL != callBack) {
		if (wateringZone < ZONE_COUNT) {
			callBack->onZoneChanged(wateringZone, false);
			wateringZone = INVALID_ZONE;
		}

		callBack->onProgramStop();
	}
}

unsigned Program::getWateringZone(time_t rawTime) const {
/*
	time_t zoneStart = wateringStart;
	time_t zoneEnd;

	for (unsigned i = 0; i < ZONE_COUNT; i++) {
		zoneEnd = zoneStart + getRunTime_noSafe(i);

		if (zoneStart <= rawTime && rawTime < zoneEnd) {
			return i;
		}

		zoneStart = zoneEnd;
	}
*/
	return INVALID_ZONE;
}

bool Program::periodic(time_t rawTime) {

	std::lock(wateringStartMutex, runTimeMutex);
	std::lock_guard<std::mutex> lk1(wateringStartMutex, std::adopt_lock);
	std::lock_guard<std::mutex> lk2(runTimeMutex, std::adopt_lock);

	if (rawTime < wateringStart) {
		throw std::invalid_argument("Invalid time");
	}

	if (0 == wateringStart) {
		return false;
	}

	unsigned newZone = getWateringZone(rawTime);

	if (wateringZone != newZone) {
		if (wateringZone < ZONE_COUNT) {
			callBack->onZoneChanged(wateringZone, false);
		}

		wateringZone = newZone;

		if (wateringZone < ZONE_COUNT) {
			callBack->onZoneChanged(wateringZone, true);
		}
	}
}

bool Program::isScheduled(time_t rawTime) const {
	bool result = false;
/*
	switch (wateringType) {
	case PERIOD:
		result = isPeriodScheduled(rawTime);
		break;
	case SPECIFIED:
		result = isSpecifiedScheduled(rawTime);
		break;
	default:
		throw std::domain_error("Invalid scheduler type");
	}
*/
	return result;
}

bool Program::isPeriodScheduled(time_t rawTime) const {
	return false;
}

bool Program::isSpecifiedScheduled(time_t rawTime) const {
	struct tm * timeinfo;
	timeinfo = localtime(&rawTime);

	std::lock(startTimeMutex, days.mutex);
	std::lock_guard<std::mutex> lk1(startTimeMutex, std::adopt_lock);
	std::lock_guard<std::mutex> lk2(days.mutex, std::adopt_lock);
/*
	for (auto it = startTimes.begin();  startTimes.end() != it; ++it) {
		int hour = (*it) / (100*100);
		int min = (*it) / 100 % 100;
		int sec = (*it) % 100;

		if (timeinfo->tm_hour == hour &&
			timeinfo->tm_min == min &&
			timeinfo->tm_sec == sec &&
			getDay_noSafe(timeinfo->tm_wday)) {
			return true;
		}
	}
*/
	return false;
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	std::lock_guard<std::mutex> guard(name.mutex);
	return name.object;
}

void Program::setName(const std::string& newName) {
	std::lock_guard<std::mutex> guard(name.mutex);
	name.object = newName;
}

////////////////////////////////////////////////////////////////
// DAY

bool Program::getDay_noSafe(unsigned dayId) const {
	if (days.object.size() <= dayId) {
		throw std::invalid_argument("Invalid dayID");
	}

	return days.object[dayId];
}

void Program::enableDay(unsigned dayId, bool enable) {
	std::lock_guard<std::mutex> guard(days.mutex);

	if (days.object.size() <= dayId) {
		throw std::invalid_argument("Invalid dayID");
	}

	days.object[dayId] = enable;
}

bool Program::isDayEnabled(unsigned dayIdx) const {
/*
	std::lock_guard<std::mutex> guard(dayMutex);
	return getDay_noSafe(dayIdx);
*/
	return false;
}

////////////////////////////////////////////////////////////////
// RUN TIME

unsigned Program::getRunTime_noSafe(unsigned runTimeId) const {
	if (runTimes.size() <= runTimeId) {
		throw std::invalid_argument("Invalid runTimeID");
	}

	return runTimes[runTimeId];
}

void Program::setRunTime(unsigned runTimeId, unsigned minutes) {
	std::lock_guard<std::mutex> guard(runTimeMutex);

	if (runTimes.size() <= runTimeId) {
		throw std::invalid_argument("Invalid runTimeID");
	}

	runTimes[runTimeId] = minutes;
}

unsigned Program::getRunTime(unsigned runTimeId) const {
/*
	std::lock_guard<std::mutex> guard(runTimeMutex);
	return getRunTime_noSafe(runTimeId);
*/
	return false;
}

////////////////////////////////////////////////////////////////
// START TIME

Program::StartTimes::const_iterator Program::getStartTime_noSafe(unsigned startTimeId) const {
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it) {
		if (startTimeId == it->first) {
			return it;
		}
	}

	throw std::invalid_argument("Invalid startTimeID");
}

unsigned Program::addStartTime(unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);
	startTimes.push_back(std::make_pair(nextStartTimeID++, minutes));
	return startTimes.back().first;
}

void Program::deleteStartTime(unsigned startTimeId) {
	std::lock_guard<std::mutex> guard(startTimeMutex);
	auto it = getStartTime_noSafe(startTimeId);
	startTimes.erase(it);
}

void Program::setStartTime(unsigned startTimeId, unsigned minutes) {
/*
	std::lock_guard<std::mutex> guard(startTimeMutex);
	auto it = getStartTime_noSafe(startTimeId);
	it->second = minutes;
*/
}

unsigned Program::getStartTime(unsigned startTimeId) const {
	std::lock_guard<std::mutex> guard(startTimeMutex);
	auto it = getStartTime_noSafe(startTimeId);
	return it->second;
}
