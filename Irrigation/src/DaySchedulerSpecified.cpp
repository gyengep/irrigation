/*
 * WateringSpecified.cpp
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "DaySchedulerSpecified.h"

#include "exception.h"


DayScheduler_Specified::DayScheduler_Specified() {
	days.fill(false);
}

DayScheduler_Specified::~DayScheduler_Specified() {
}

void DayScheduler_Specified::enableDay(IdType id, bool enable) {
	if (DAY_COUNT <= id) {
		throw invalid_id(INVALID_DAYID);
	}

	days[id] = enable;
}

bool DayScheduler_Specified::isDayEnabled(IdType id) const {
	if (DAY_COUNT <= id) {
		throw invalid_id(INVALID_DAYID);
	}

	return days[id];
}

bool DayScheduler_Specified::isDayScheduled(std::tm* timeinfo) const {
	return days[timeinfo->tm_wday];
}
