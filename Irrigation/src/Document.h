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

#include "common.h"
#include "Program.h"
#include "Tools.h"


#define AUTO_LOCK_PROGRAMS(document) std::lock_guard<std::mutex> lock((document)->getProgramMutex());

class Valve;
class View;


class Document {

public:
	typedef std::list<std::pair<IdType, Program*>> ProgramList;

private:

	typedef std::array<unsigned, ZONE_COUNT> WateringTimes;

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

	// Watering
	mutable std::mutex wateringMutex;
	std::time_t wateringStart;
	WateringTimes wateringTimes;
	IdType wateringZone;

	void openValve_notSafe(IdType id, bool open);

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
	std::mutex& getProgramMutex() { return programMutex; }
	const ProgramList& getPrograms() const;
	Program& addProgram();
	void deleteProgram(IdType id);
	void moveProgram(IdType id, unsigned newPosition);
	Program& getProgram(IdType id);
	const Program& getProgram(IdType id) const;

	// Zone, Valve
	void openZone(IdType id, bool open);
	void openValve(IdType id, bool open);

	// View
	void addView(View* view);
	void updateViews();
};

#endif /* DOCUMENT_H_ */
