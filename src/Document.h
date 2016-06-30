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
#include <shared_mutex>

#include "Program.h"
#include "SortedMap.h"

class Valve;
class View;

class Document : public SchedulerCallBack {

public:
	typedef SortedMap<unsigned, Program*> ProgramList;

private:
	static const unsigned ZONE_COUNT = 6;
	static const unsigned VALVE_COUNT = ZONE_COUNT + 1;

	// Views
	mutable std::mutex viewMutex;
	std::list<View*> views;

	// Valves
	mutable std::mutex valveMutex;
	std::array<Valve*, VALVE_COUNT> valves;

	// Programs
	mutable std::shared_timed_mutex programMutex;
	unsigned nextProgramID;
	ProgramList programs;
	ProgramList deletedPrograms;

public:

	Document();
	virtual ~Document();

	// Program
	const ProgramList& getPrograms() const;
	void releasePrograms() const;
	Program& addProgram();
	void deleteProgram(unsigned id);
	void moveProgram(unsigned id, unsigned newIdx);
	Program& getProgram(unsigned id);

	// Zone
	unsigned getZoneCount() const { return ZONE_COUNT; }
	void openZone(unsigned idx, bool open);

	// Valve
	unsigned getValveCount() const { return VALVE_COUNT; }
	void openValve(unsigned idx, bool open);

	// View
	void addView(View* view);
	void updateViews();

	// from SchedulerCallBack
	virtual void onProgramStart() {}
	virtual void onProgramStop() {}
	virtual void onZoneChanged(unsigned zoneIdx, bool enable) {}

};

#endif /* DOCUMENT_H_ */
