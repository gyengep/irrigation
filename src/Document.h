/*
 * Document.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <list>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "Program.h"

class Valve;
class View;

class Document : public SchedulerCallBack {

public:
	typedef std::list<Program*> ProgramList;

private:
	static const unsigned ZONE_COUNT = 6;
	static const unsigned VALVE_COUNT = ZONE_COUNT + 1;

	std::list<View*> views;
	std::vector<Valve*> valves;
	ProgramList programs;
	ProgramList deletedPrograms;

	mutable std::mutex viewMutex;
	mutable std::mutex valveMutex;
	mutable std::shared_timed_mutex programMutex;

	ProgramList::const_iterator getProgram_notSafe(unsigned id) const;

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
