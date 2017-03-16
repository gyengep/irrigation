/*
 * Document.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <list>
#include <array>
#include <mutex>

#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Logic/Zones.h"


class Valve;
class View;


class Document {

	typedef std::array<unsigned, ZONE_COUNT> WateringTimes;

	// Views
	mutable std::mutex viewMutex;
	std::list<View*> views;

	ProgramContainer programs;
	Zones zones;

	// Watering
	mutable std::mutex wateringMutex;
	std::time_t wateringStart;
	WateringTimes wateringTimes;
	IdType wateringZone;

	void openValve_notSafe(IdType id, bool open);

	bool isWateringActive_notSafe() const;
	IdType getWateringZone_notSafe(std::time_t rawTime) const;
	bool startWatering_notSafe(Program& program, std::time_t rawTime);
	void stopWatering_notSafe();
	
public:

	Document();
	virtual ~Document();

	void doTask();

	// Watering
	bool isWateringActive() const;
	void startWatering(IdType programId);
	void stopWatering();

	// Program
	const ProgramContainer& getPrograms() const { return programs; }
	ProgramContainer& getPrograms() { return programs; }

	// Zones
	const Zones& getZones() const { return zones; }
	Zones& getZones() { return zones; }

	// View
	void addView(View* view);
	void updateViews();
};

#endif /* DOCUMENT_H_ */
