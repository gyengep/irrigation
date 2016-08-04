/*
 * Document.cpp
 *
 *  Created on: 2016. máj. 15.
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
		std::lock_guard<std::mutex> guard(viewMutex);
		for (auto it = views.begin(); views.end() != it; ++it) {
			delete (*it);
		}
		views.clear();
	}

	if (true) {
		std::lock_guard<std::mutex> guard(valveMutex);
		for (auto it = valves.begin(); valves.end() != it; ++it) {
			delete (*it);
		}
	}
}

void Document::doTask() {
	std::time_t rawTime = getApplication()->getTime();

	std::lock_guard<std::mutex> lock(wateringMutex);

	if (!isWateringActive()) {
		Program* scheduledProgram = NULL;
		const ProgramList& programList = getPrograms();

		for (auto it = programList.begin(); programList.end() != it; ++it) {
			Program* program = it->second;
			if (program->isScheduled(rawTime)) {
				scheduledProgram = program;
				break;
			}
		}

		releasePrograms();

		if (scheduledProgram) {
			startWatering_notSafe(*scheduledProgram, rawTime);
		}
	}

	if (isWateringActive()) {
		IdType zone = getWateringZone(rawTime);

		if (wateringZone != zone) {
			if (wateringZone < ZONE_COUNT) {
				openZone(wateringZone, false);
			}

			wateringZone = zone;
			if (wateringZone < ZONE_COUNT) {
				openZone(wateringZone, true);
			} else {
				stopWatering_notSafe();
			}
		}
	}
}

/////////////////////////////////////////////////////
// Watering


IdType Document::getWateringZone(std::time_t rawTime) const {
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

void Document::startWatering(IdType programId) {
	Program& program = getProgram(programId);

	std::lock_guard<std::mutex> lock(wateringMutex);
	startWatering_notSafe(program, getApplication()->getTime());
}

void Document::startWatering_notSafe(Program& program, std::time_t rawTime) {
	const Program::RunTimes& runTimes = program.getRunTimes();

	if (runTimes.size() != wateringTimes.size()) {
		throw std::runtime_error("runTimes.length() != wateringTimes.length()");
	}

	for (unsigned i = 0; i < wateringTimes.size(); i++) {
		wateringTimes[i].store(runTimes[i].second);
	}

	program.releaseRunTimes();

	wateringZone = ZONE_COUNT;
	wateringStart = rawTime;
}

void Document::stopWatering() {
	std::lock_guard<std::mutex> lock(wateringMutex);
	stopWatering_notSafe();
}

void Document::stopWatering_notSafe() {
	if (ZONE_COUNT != wateringZone) {
		openZone(wateringZone, false);
	}

	wateringZone = ZONE_COUNT;
}


/////////////////////////////////////////////////////
// Program

const Document::ProgramList& Document::getPrograms() const {
	programMutex.lock();
	return programs;
}

void Document::releasePrograms() const {
	programMutex.unlock();
}

Program& Document::addProgram() {
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program = new Program();
	tools::push_back(programs, nextProgramId, program);
	nextProgramId++;
	return *program;
}

void Document::deleteProgram(IdType id) {
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program;
	try {
		program = tools::erase(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	deletedPrograms.push_back(program);
}

void Document::moveProgram(IdType id, unsigned newPosition) {
	std::lock_guard<std::mutex> lock(programMutex);

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
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	return *program;
}

const Program& Document::getProgram(IdType id) const {
	std::lock_guard<std::mutex> lock(programMutex);

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
	std::lock_guard<std::mutex> guard(viewMutex);
	views.push_back(view);
}

void Document::updateViews() {
	std::lock_guard<std::mutex> guard(viewMutex);
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
	
	std::lock_guard<std::mutex> guard(valveMutex);
	openValve_notSafe(id, open);
	openValve_notSafe(getZoneCount(), open);
}

void Document::openValve(IdType id, bool open) {
	if (getValveCount() <= id) {
		throw std::out_of_range(INVALID_VALVEID);
	} 
	
	std::lock_guard<std::mutex> guard(valveMutex);
	openValve_notSafe(id, open);
}
