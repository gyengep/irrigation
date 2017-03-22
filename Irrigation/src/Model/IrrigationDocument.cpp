#include "common.h"
#include "IrrigationDocument.h"

#include "Hardware/Valves.h"
#include "Logic/Program.h"
#include "Model/Application.h"


#define AUTO_LOCK_WATERING() std::lock_guard<std::mutex> lockValve(wateringMutex)

IrrigationDocument::IrrigationDocument() :
	zones(MASTER_VALVE_ID, ZONE_VALVE_IDS)
{
	wateringZone = ZONE_COUNT;
	wateringStart = 0;
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::load() {
	Program* program;

	program = new Program();
	program->setName("fulocsolas");
	program->getRunTimes().at(0) = 25;
	program->getRunTimes().at(1) = 35;
	program->getRunTimes().at(2) = 25;
	program->getStartTimes().insert(StartTime(4, 0));
	getPrograms().insert(program);

	program = new Program();
	program->setName("buxus");
	program->getRunTimes().at(3) = 20;
	program->getStartTimes().insert(StartTime(6, 0));
	getPrograms().insert(program);

	program = new Program();
	program->setName("paradicsom");
	program->getRunTimes().at(4) = 3;
	program->getStartTimes().insert(StartTime(8, 0));
	getPrograms().insert(program);
}

void IrrigationDocument::doTask() {
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

bool IrrigationDocument::isWateringActive() const {
	AUTO_LOCK_WATERING();
	return isWateringActive_notSafe();
}

void IrrigationDocument::startWatering(IdType programId) {
/*
	AUTO_LOCK_WATERING();
	AUTO_LOCK_PROGRAMS();
	Program& program = programs().get(programId);
	startWatering_notSafe(program, getApplication()->getTime());
*/
}

void IrrigationDocument::stopWatering() {
	AUTO_LOCK_WATERING();
	stopWatering_notSafe();
}

bool IrrigationDocument::isWateringActive_notSafe() const {
	return (ZONE_COUNT > wateringZone);
}

bool IrrigationDocument::startWatering_notSafe(Program& program, std::time_t rawTime) {
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

void IrrigationDocument::stopWatering_notSafe() {
/*
	if (ZONE_COUNT > wateringZone) {
		openZone(wateringZone, false);
	}

	wateringZone = ZONE_COUNT;
	wateringStart = 0;
*/
}

IdType IrrigationDocument::getWateringZone_notSafe(std::time_t rawTime) const {
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
