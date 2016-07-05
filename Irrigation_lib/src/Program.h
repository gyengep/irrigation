/*
 * Program.h
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <atomic>
#include <utility>
#include <mutex>
#include <string>
#include <vector>
#include <list>
#include <climits>

#include "common.h"

class DayScheduler;


class SchedulerCallBack {
public:
	virtual void onProgramStart() = 0;
	virtual void onProgramStop() = 0;
	virtual void onZoneChanged(unsigned zoneIdx, bool enable) = 0;
};

class Program {

	static const unsigned ZONE_COUNT = 6;

public:

	typedef std::array<std::pair<IdType, std::atomic<unsigned>>, ZONE_COUNT> RunTimes;
	typedef std::list<std::pair<IdType, unsigned>> StartTimes;


	enum WateringType {
		SPECIFIED,

		LAST
	};

private:


	SchedulerCallBack* callBack;

	// Name
	mutable std::mutex nameMutex;
	std::string name;

	// Watering day
	mutable std::mutex wateringMutex;
	WateringType wateringType;
	std::array<DayScheduler*, WateringType::LAST> watering;

	// Runtime
	mutable std::mutex runTimeMutex;
	RunTimes runTimes;

	// StartTime
	IdType nextStartTimeId;
	mutable std::mutex startTimeMutex;
	StartTimes startTimes;

	bool isDayScheduled() const;

public:
	Program(SchedulerCallBack* callBack);
	~Program();

	std::string getName() const;
	void setName(const std::string& name);

	bool isScheduled(time_t rawTime) const;
	void start(time_t rawTime);
	void stop();

	// Runtime
	const RunTimes& getRunTimes() const;
	void releaseRunTimes() const;
	void setRunTime(IdType id, unsigned minutes);
	unsigned getRunTime(IdType id) const;

	// StartTime
	const StartTimes& getStartTimes() const;
	void releaseStartTimes() const;
	IdType addStartTime(unsigned minutes);
	void deleteStartTime(IdType id);
	void setStartTime(IdType id, unsigned minutes);
	unsigned getStartTime(IdType id) const;
};

#endif /* PROGRAM_H_ */
