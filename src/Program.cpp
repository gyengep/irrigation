/*
 * Program.cpp
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#include <stdexcept>
#include "Program.h"

unsigned Program::nextID = 0;


Program::Program(SchedulerCallBack* callBack) :
	callBack(callBack),
	wateringType(SPECIFIED),
	wateringStart(0),
	wateringZone(INVALID_ZONE),
	id(nextID++)
{
	days.resize(DAY_COUNT, false);
	runTimes.resize(ZONE_COUNT, 0);
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
	time_t zoneStart = wateringStart;
	time_t zoneEnd;

	for (unsigned i = 0; i < ZONE_COUNT; i++) {
		zoneEnd = zoneStart + getRunTime_noSafe(i);

		if (zoneStart <= rawTime && rawTime < zoneEnd) {
			return i;
		}

		zoneStart = zoneEnd;
	}

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
	bool result;

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

	return result;
}

bool Program::isPeriodScheduled(time_t rawTime) const {
	return false;
}

bool Program::isSpecifiedScheduled(time_t rawTime) const {
	struct tm * timeinfo;
	timeinfo = localtime(&rawTime);

	std::lock(startTimeMutex, dayMutex);
	std::lock_guard<std::mutex> lk1(startTimeMutex, std::adopt_lock);
	std::lock_guard<std::mutex> lk2(dayMutex, std::adopt_lock);

	std::vector<unsigned>::const_iterator it;
	for (it = startTimes.begin();  startTimes.end() != it; ++it) {
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

	return false;
}

////////////////////////////////////////////////////////////////
// NAME

std::string Program::getName() const {
	std::lock_guard<std::mutex> guard(nameMutex);
	return name;
}

void Program::setName(const std::string& name) {
	std::lock_guard<std::mutex> guard(nameMutex);
	this->name = name;
}

////////////////////////////////////////////////////////////////
// TYPE

void Program::setType(Type type) {
	std::lock_guard<std::mutex> guard(typeMutex);
	this->wateringType = type;
}

Program::Type Program::getType() const {
	std::lock_guard<std::mutex> guard(typeMutex);
	return wateringType;
}

////////////////////////////////////////////////////////////////
// DAY

bool Program::getDay_noSafe(unsigned dayIdx) const {
	if (days.size() <= dayIdx) {
		throw std::out_of_range("Day index out of range");
	}

	return days[dayIdx];
}

void Program::enableDay(unsigned dayIdx, bool enable) {
	std::lock_guard<std::mutex> guard(dayMutex);

	if (days.size() <= dayIdx) {
		throw std::out_of_range("Day index out of range");
	}

	days[dayIdx] = enable;
}

bool Program::isDayEnabled(unsigned dayIdx) const {
	std::lock_guard<std::mutex> guard(dayMutex);
	return getDay_noSafe(dayIdx);
}

////////////////////////////////////////////////////////////////
// RUN TIME

unsigned Program::getRunTime_noSafe(unsigned runTimeId) const {
	if (runTimes.size() <= runTimeId) {
		throw std::out_of_range("RunTime index out of range");
	}

	return runTimes[runTimeId];
}

void Program::setRunTime(unsigned runTimeId, unsigned minutes) {
	std::lock_guard<std::mutex> guard(runTimeMutex);

	if (runTimes.size() <= runTimeId) {
		throw std::out_of_range("RunTime index out of range");
	}

	runTimes[runTimeId] = minutes;
}

unsigned Program::getRunTime(unsigned runTimeId) const {
	std::lock_guard<std::mutex> guard(runTimeMutex);
	return getRunTime_noSafe(runTimeId);
}

////////////////////////////////////////////////////////////////
// START TIME

unsigned Program::addStartTime(unsigned startTimeId, unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);
	startTimes.push_back(minutes);
	return startTimes.size() - 1;
}

void Program::deleteStartTime(unsigned startTimeId) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	if (startTimes.size() <= startTimeId) {
		throw std::out_of_range("StartTime index out of range");
	}

	startTimes.erase(startTimes.begin() + startTimeId);
}

void Program::setStartTime(unsigned startTimeId, unsigned minutes) {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	if (startTimes.size() <= startTimeId) {
		throw std::out_of_range("StartTime index out of range");
	}

	startTimes[startTimeId] = minutes;
}

unsigned Program::getStartTime(unsigned startTimeId) const {
	std::lock_guard<std::mutex> guard(startTimeMutex);

	if (startTimes.size() <= startTimeId) {
		throw std::out_of_range("StartTime index out of range");
	}

	return startTimes[startTimeId];
}
