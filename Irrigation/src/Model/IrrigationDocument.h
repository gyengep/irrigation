#pragma once

#include <array>
#include <mutex>

#include "DocumentView/Document.h"
#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Logic/Zones.h"


class IrrigationDocument : public Document {

	typedef std::array<unsigned, ZONE_COUNT> WateringTimes;

	std::unique_ptr<ProgramContainer> programs;
	Zones zones;

	// Watering
	mutable std::mutex wateringMutex;
	std::time_t wateringStart;
	WateringTimes wateringTimes;
	IdType wateringZone;

	bool isWateringActive_notSafe() const;
	IdType getWateringZone_notSafe(std::time_t rawTime) const;
	bool startWatering_notSafe(Program& program, std::time_t rawTime);
	void stopWatering_notSafe();
	
public:

	IrrigationDocument();
	virtual ~IrrigationDocument();

	void doTask();

	// Watering
	bool isWateringActive() const;
	void startWatering(IdType programId);
	void stopWatering();

	// Program
	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	// Zones
	const Zones& getZones() const { return zones; }
	Zones& getZones() { return zones; }

	virtual void load();
};
