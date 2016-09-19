/*
 * WateringSpecified.cpp
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Scheduler.h"

#include "exception.h"


SpecifiedScheduler::SpecifiedScheduler() {
	days.fill(false);
}

SpecifiedScheduler::~SpecifiedScheduler() {
}

void SpecifiedScheduler::enableDay(IdType id, bool enable) {
	if (DAY_COUNT <= id) {
		throw invalid_id(INVALID_DAYID);
	}

	days[id] = enable;
}

bool SpecifiedScheduler::isDayEnabled(IdType id) const {
	if (DAY_COUNT <= id) {
		throw invalid_id(INVALID_DAYID);
	}

	return days[id];
}

bool SpecifiedScheduler::isScheduled(std::tm* timeinfo) const {
	return days[timeinfo->tm_wday];
}
