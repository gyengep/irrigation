/*
 * WateringSpecified.cpp
 *
 *  Created on: 2016. j�l. 3.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "Scheduler.h"



SpecifiedScheduler::SpecifiedScheduler() {
	days.fill(false);
}

SpecifiedScheduler::~SpecifiedScheduler() {
}

void SpecifiedScheduler::enableDay(IdType id, bool enable) {
	if (DAY_COUNT <= id) {
		throw InvalidDayException();
	}

	days[id] = enable;
}

bool SpecifiedScheduler::isDayEnabled(IdType id) const {
	if (DAY_COUNT <= id) {
		throw InvalidDayException();
	}

	return days[id];
}

bool SpecifiedScheduler::isScheduled(const std::tm& timeinfo) const {
	// TODO range check
	return days[timeinfo.tm_wday];
}