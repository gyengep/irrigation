/*
 * Program.h
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <utility>
#include <mutex>
#include <string>
#include <vector>
#include <list>
#include <climits>

#include "LockableObject.h"

class SchedulerCallBack {
public:
	virtual void onProgramStart() = 0;
	virtual void onProgramStop() = 0;
	virtual void onZoneChanged(unsigned zoneIdx, bool enable) = 0;
};

class Program {

	static const unsigned DAY_COUNT = 7;
	static const unsigned ZONE_COUNT = 6;

	static const unsigned INVALID_ZONE = UINT_MAX;


	SchedulerCallBack* callBack;

	mutable std::mutex wateringStartMutex;
	time_t wateringStart;

	unsigned wateringZone;

	static unsigned nextProgramID;
	static unsigned nextStartTimeID;

	typedef std::list<std::pair<unsigned, unsigned>> StartTimes;
	typedef std::array<bool, 7> Days;
	typedef std::array<unsigned, ZONE_COUNT> RunTimes;

	const unsigned id;

	LockableObject<std::string> name;
	LockableObject<Days> days;

	// Day
	//mutable std::mutex dayMutex;
	//Days days;

	// Runtime
	mutable std::mutex runTimeMutex;
	RunTimes runTimes;

	// StartTime
	mutable std::mutex startTimeMutex;
	StartTimes startTimes;



	bool isPeriodScheduled(time_t rawTime) const;
	bool isSpecifiedScheduled(time_t rawTime) const;
	unsigned getWateringZone(time_t rawTime) const;

	bool getDay_noSafe(unsigned dayId) const;
	unsigned getRunTime_noSafe(unsigned runTimeId) const;
	StartTimes::const_iterator getStartTime_noSafe(unsigned startTimeId) const;

public:
	Program(SchedulerCallBack* callBack);
	~Program();

	unsigned getID() const { return id; }

	std::string getName() const;
	void setName(const std::string& name);

	bool isScheduled(time_t rawTime) const;
	void start(time_t rawTime);
	void stop();
	bool periodic(time_t rawTime);

	// PERIOD
	//void setSkipDays(unsigned skipDays) { this->skipDays = skipDays; }
	//unsigned getSkipDays(void) const { return skipDays; }

	//SPECIFIED
	void enableDay(unsigned dayIdx, bool enable);
	bool isDayEnabled(unsigned dayIdx) const;

	void setRunTime(unsigned runTimeId, unsigned minutes);
	unsigned getRunTime(unsigned runTimeId) const;

	unsigned addStartTime(unsigned minutes);
	void deleteStartTime(unsigned startTimeId);
	void setStartTime(unsigned startTimeId, unsigned minutes);
	unsigned getStartTime(unsigned startTimeId) const;
};

#endif /* PROGRAM_H_ */
