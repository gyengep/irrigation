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

#include "Program.h"
#include "Tools.h"

class Valve;
class View;


class Document {

public:
	typedef std::list<std::pair<IdType, Program*>> ProgramList;

private:
	static const unsigned ZONE_COUNT = 6;
	static const unsigned VALVE_COUNT = ZONE_COUNT + 1;

	typedef std::array<std::atomic<unsigned>, ZONE_COUNT> WateringTimes;

	// Views
	mutable std::mutex viewMutex;
	std::list<View*> views;

	// Valves
	mutable std::mutex valveMutex;
	std::array<Valve*, VALVE_COUNT> valves;

	// Programs
	mutable std::mutex programMutex;
	IdType nextProgramId;
	ProgramList programs;
	std::list<Program*> deletedPrograms;

	// Watering
	mutable std::mutex wateringMutex;
	std::atomic<std::time_t> wateringStart;
	WateringTimes wateringTimes;
	IdType wateringZone;

	void openValve_notSafe(IdType id, bool open);

	IdType getWateringZone(std::time_t rawTime) const;
	bool isWateringActive_notSafe() const;
	void startWatering_notSafe(Program& program, std::time_t rawTime);
	void stopWatering_notSafe();
	
public:

	Document();
	virtual ~Document();

	void doTask();

	// Watering
	bool isWateringActive() const { return (0 != wateringStart); }
	void startWatering(IdType programId);
	void stopWatering();

	// Program
	const ProgramList& getPrograms() const;
	void releasePrograms() const;
	Program& addProgram();
	void deleteProgram(IdType id);
	void moveProgram(IdType id, unsigned newPosition);
	Program& getProgram(IdType id);
	const Program& getProgram(IdType id) const;

	// Zone, Valve
	unsigned getZoneCount() const { return ZONE_COUNT; }
	unsigned getValveCount() const { return VALVE_COUNT; }
	void openZone(IdType id, bool open);
	void openValve(IdType id, bool open);

	// View
	void addView(View* view);
	void updateViews();
};

#endif /* DOCUMENT_H_ */
