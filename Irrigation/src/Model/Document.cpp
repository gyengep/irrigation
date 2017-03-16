/*
 * Document.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Document.h"
#include "Application.h"

#include "Logic/Program.h"
#include "Hardware/Valves.h"
#include "Model/View.h"


#define AUTO_LOCK_VIEW() std::lock_guard<std::mutex> lockView(viewMutex)
#define AUTO_LOCK_WATERING() std::lock_guard<std::mutex> lockValve(wateringMutex)

Document::Document() :
	zones(MASTER_VALVE_ID, ZONE_VALVE_IDS)
{

	wateringZone = ZONE_COUNT;
	wateringStart = 0;
}

Document::~Document() {
	if (true) {
		AUTO_LOCK_VIEW();
		for (auto it = views.begin(); views.end() != it; ++it) {
			delete (*it);
		}
		views.clear();
	}

}

void Document::doTask() {
	/*
	std::time_t rawTime = getApplication()->getTime();

	AUTO_LOCK_WATERING();

	if (!isWateringActive_notSafe()) {

		AUTO_LOCK_PROGRAMS();

		const Programs& programList = programContainer.container();

		for (auto it = programList.begin(); programList.end() != it; ++it) {
			Program* program = it->second;
			if (program->isScheduled(rawTime)) {
				if (startWatering_notSafe(*program, rawTime)) {
					break;
				}
			}
		}

	} else {

		IdType newZone = getWateringZone_notSafe(rawTime);

		if (wateringZone != newZone) {
			if (ZONE_COUNT > wateringZone) {
				openZone(wateringZone, false);
			}

			wateringZone = newZone;
			if (ZONE_COUNT > wateringZone) {
				openZone(wateringZone, true);
			} else {
				stopWatering_notSafe();
			}
		}
	}
	*/
}

/////////////////////////////////////////////////////
// Watering

bool Document::isWateringActive() const {
	AUTO_LOCK_WATERING();
	return isWateringActive_notSafe();
}

void Document::startWatering(IdType programId) {
/*
	AUTO_LOCK_WATERING();
	AUTO_LOCK_PROGRAMS();
	Program& program = programs().get(programId);
	startWatering_notSafe(program, getApplication()->getTime());
*/
}

void Document::stopWatering() {
	AUTO_LOCK_WATERING();
	stopWatering_notSafe();
}

bool Document::isWateringActive_notSafe() const {
	return (ZONE_COUNT > wateringZone);
}

bool Document::startWatering_notSafe(Program& program, std::time_t rawTime) {
/*
	const Program::RunTimes& runTimes = program.runTimes().container();

	if (runTimes.size() != wateringTimes.size()) {
		throw std::runtime_error("runTimes.length() != wateringTimes.length()");
	}

	stopWatering_notSafe();

	for (unsigned i = 0; i < wateringTimes.size(); i++) {
		wateringTimes[i] = runTimes[i].second;

		if ((ZONE_COUNT > wateringZone) && 0 < wateringTimes[i]) {
			wateringZone = i;
			wateringStart = rawTime;
			openZone(wateringZone, true);
		}
	}
*/
	return (ZONE_COUNT > wateringZone);
}

void Document::stopWatering_notSafe() {
/*
	if (ZONE_COUNT > wateringZone) {
		openZone(wateringZone, false);
	}

	wateringZone = ZONE_COUNT;
	wateringStart = 0;
*/
}

IdType Document::getWateringZone_notSafe(std::time_t rawTime) const {
	if (0 == wateringStart) {
		throw std::runtime_error("Watering doesn't started");
	}

	if (rawTime < wateringStart) {
		throw std::runtime_error("Invalid time");
	}

	std::time_t t = wateringStart;
	for (unsigned zone = 0; zone < ZONE_COUNT; ++zone) {
		t += wateringTimes[zone];

		if (rawTime < t) {
			return (IdType)zone;
		}
	}

	return ZONE_COUNT;
}

/////////////////////////////////////////////////////
// View

void Document::addView(View* view) {
	AUTO_LOCK_VIEW();
	views.push_back(view);
}

void Document::updateViews() {
	AUTO_LOCK_VIEW();
	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->update();
	}
}
