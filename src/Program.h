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


class SchedulerCallBack {
public:
	virtual void onProgramStart() = 0;
	virtual void onProgramStop() = 0;
	virtual void onZoneChanged(unsigned zoneIdx, bool enable) = 0;
};

class Program {

	static const unsigned DAY_COUNT = 7;
	static const unsigned ZONE_COUNT = 6;

	SchedulerCallBack* callBack;

	static unsigned nextProgramID;
	static unsigned nextStartTimeID;

	typedef std::array<std::pair<IdType, std::atomic<bool>>, 7> Days;
	typedef std::array<std::pair<IdType, std::atomic<unsigned>>, ZONE_COUNT> RunTimes;
	typedef std::list<std::pair<IdType, unsigned>> StartTimes;

	// Name
	mutable std::mutex nameMutex;
	std::string name;

	// Day
	mutable std::mutex dayMutex;
	Days days;

	// Runtime
	mutable std::mutex runTimeMutex;
	RunTimes runTimes;

	// StartTime
	mutable std::mutex startTimeMutex;
	StartTimes startTimes;
/*


	bool isPeriodScheduled(time_t rawTime) const;
	bool isSpecifiedScheduled(time_t rawTime) const;
	unsigned getWateringZone(time_t rawTime) const;

*/

public:
	Program(SchedulerCallBack* callBack);
	~Program();

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
	void enableDay(IdType id, bool enable);
	bool isDayEnabled(IdType id) const;

	// Runtime
	void setRunTime(IdType id, unsigned minutes);
	unsigned getRunTime(IdType id) const;

	// StartTime
	unsigned addStartTime(unsigned minutes);
	void deleteStartTime(IdType id);
	void setStartTime(IdType id, unsigned minutes);
	unsigned getStartTime(IdType id) const;
};

#endif /* PROGRAM_H_ */
