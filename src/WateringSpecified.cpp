/*
 * WateringSpecified.cpp
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "WateringSpecified.h"
#include "Tools.h"


Watering_Specified::Watering_Specified() {
	for (IdType i = 0; i < days.size(); i++) {
		days[i] = std::make_pair(i, false);
	}
}

Watering_Specified::~Watering_Specified() {
}

void Watering_Specified::enableDay(IdType id, bool enable) {
	//std::lock_guard<std::mutex> guard(wateringMutex);

	try {
		tools::set(days, id, enable);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}
}

bool Watering_Specified::isDayEnabled(IdType id) const {
	//std::lock_guard<std::mutex> guard(wateringMutex);

	bool result;
	try {
		result = tools::get(days, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_DAYID);
	}

	return result;
}

bool Watering_Specified::isDayScheduled(std::tm* timeinfo) const {
	//std::lock_guard<std::mutex> guard(wateringMutex);
	return days[timeinfo->tm_wday].second.load();
}
