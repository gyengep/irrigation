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


class DayScheduler;


class SchedulerCallBack {
public:
	virtual void onProgramStart() = 0;
	virtual void onProgramStop() = 0;
	virtual void onZoneChanged(unsigned zoneIdx, bool enable) = 0;
};

class Program {
public:

	enum WateringType {
		SPECIFIED,

		LAST
	};

private:

	static const unsigned ZONE_COUNT = 6;

	SchedulerCallBack* callBack;

	typedef std::array<std::pair<IdType, std::atomic<unsigned>>, ZONE_COUNT> RunTimes;
	typedef std::list<std::pair<IdType, unsigned>> StartTimes;

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
	IdType nextStartTimeID;
	mutable std::mutex startTimeMutex;
	StartTimes startTimes;
/*
	unsigned getWateringZone(time_t rawTime) const;
*/

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
	void setRunTime(IdType id, unsigned minutes);
	unsigned getRunTime(IdType id) const;

	// StartTime
	IdType addStartTime(unsigned minutes);
	void deleteStartTime(IdType id);
	void setStartTime(IdType id, unsigned minutes);
	unsigned getStartTime(IdType id) const;
};

#endif /* PROGRAM_H_ */
