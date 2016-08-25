/*
 * Document.cpp
 *
 *  Created on: 2016. m�j. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Document.h"

#include <stdexcept>
#include <sstream>

#include "Application.h"
#include "Program.h"
#include "Valve.h"
#include "View.h"


#undef AUTO_LOCK_PROGRAMS

#define AUTO_LOCK_VIEW() std::lock_guard<std::mutex> lockView(viewMutex)
#define AUTO_LOCK_VALVE() std::lock_guard<std::mutex> lockValve(valveMutex)
#define AUTO_LOCK_PROGRAMS() std::lock_guard<std::mutex> lockView(programMutex)
#define AUTO_LOCK_WATERING() std::lock_guard<std::mutex> lockValve(wateringMutex)

Document::Document() : nextProgramId(0) {
	valves[0] = new Valve(0);
	valves[1] = new Valve(2);
	valves[2] = new Valve(3);
	valves[3] = new Valve(4);
	valves[4] = new Valve(5);
	valves[5] = new Valve(6);
	valves[6] = new Valve(27);

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

	if (true) {
		AUTO_LOCK_VALVE();
		for (auto it = valves.begin(); valves.end() != it; ++it) {
			delete (*it);
		}
	}
}

void Document::doTask() {
	std::time_t rawTime = getApplication()->getTime();

	AUTO_LOCK_WATERING();

	if (!isWateringActive()) {

		AUTO_LOCK_PROGRAMS();

		const ProgramList& programList = getPrograms();

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
}

/////////////////////////////////////////////////////
// Watering

bool Document::isWateringActive() const {
	AUTO_LOCK_WATERING();
	return (ZONE_COUNT > wateringZone);
}

void Document::startWatering(IdType programId) {
	AUTO_LOCK_WATERING();
	AUTO_LOCK_PROGRAMS();
	Program& program = getProgram(programId);
	startWatering_notSafe(program, getApplication()->getTime());
}

void Document::stopWatering() {
	AUTO_LOCK_WATERING();
	stopWatering_notSafe();
}

bool Document::startWatering_notSafe(Program& program, std::time_t rawTime) {
	const Program::RunTimes& runTimes = program.getRunTimes();

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

	return (ZONE_COUNT > wateringZone);
}

void Document::stopWatering_notSafe() {
	if (ZONE_COUNT > wateringZone) {
		openZone(wateringZone, false);
	}

	wateringZone = ZONE_COUNT;
	wateringStart = 0;
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
// Program

const Document::ProgramList& Document::getPrograms() const {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	return programs;
}

Program& Document::addProgram() {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	Program* program = new Program();
	tools::push_back(programs, nextProgramId, program);
	nextProgramId++;
	return *program;
}

void Document::deleteProgram(IdType id) {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	Program* program = NULL;

	try {
		program = tools::erase(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	delete program;
}

void Document::moveProgram(IdType id, unsigned newPosition) {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	if (programs.size() <= newPosition) {
		throw std::out_of_range("Invalid position");
	}

	Program* program;

	try {
		program = tools::erase(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	unsigned count = 0;
	auto it = programs.begin();
	while (count < newPosition) {
		++it;
		++count;
	}

	programs.insert(it, std::make_pair(id, program));
}

Program& Document::getProgram(IdType id) {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	return *program;
}

const Program& Document::getProgram(IdType id) const {
	if (programMutex.try_lock()) {
		throw std::runtime_error("Programs are not locked");
	}

	const Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	return *program;
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

/////////////////////////////////////////////////////
// Zone, Valve

void Document::openValve_notSafe(IdType id, bool open) {
#ifdef __arm__
	valves[id]->open(open);
#else
	printf("OpenValve(%lu, %s)\n", id, open ? "true" : "false");
#endif // __arm__
}

void Document::openZone(IdType id, bool open) {
	if (getZoneCount() <= id) {
		throw std::out_of_range(INVALID_ZONEID);
	} 
	
	AUTO_LOCK_VALVE();
	openValve_notSafe(id, open);
	openValve_notSafe(getZoneCount(), open);
}

void Document::openValve(IdType id, bool open) {
	if (getValveCount() <= id) {
		throw std::out_of_range(INVALID_VALVEID);
	} 
	
	AUTO_LOCK_VALVE();
	openValve_notSafe(id, open);
}
