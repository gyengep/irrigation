/*
 * WateringSpecified.cpp
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "DaySchedulerSpecified.h"

#include "Tools.h"


DayScheduler_Specified::DayScheduler_Specified() {
	for (IdType i = 0; i < days.size(); i++) {
		days[i] = std::make_pair(i, false);
	}
}

DayScheduler_Specified::~DayScheduler_Specified() {
}

void DayScheduler_Specified::enableDay(IdType id, bool enable) {
	try {
		tools::set(days, id, enable);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}
}

bool DayScheduler_Specified::isDayEnabled(IdType id) const {
	bool result;

	try {
		result = tools::get(days, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}

	return result;
}

bool DayScheduler_Specified::isDayScheduled(std::tm* timeinfo) const {
	return days[timeinfo->tm_wday].second.load();
}
