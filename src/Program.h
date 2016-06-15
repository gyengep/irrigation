/*
 * Program.h
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <mutex>
#include <string>
#include <vector>
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

	static const unsigned INVALID_ZONE = UINT_MAX;

public:

	enum Type {
		PERIOD, SPECIFIED
	};

private:

	SchedulerCallBack* callBack;
	Type wateringType;
	time_t wateringStart;
	unsigned wateringZone;

	static unsigned nextID;

	const unsigned id;
	std::string name;

	// PERIOD
	unsigned skipDays;

	// SPECIFIED
	std::vector<bool> days;
	std::vector<unsigned> startTimes;
	std::vector<unsigned> runTimes;

	mutable std::mutex wateringStartMutex;
	mutable std::mutex dayMutex;
	mutable std::mutex startTimeMutex;
	mutable std::mutex runTimeMutex;
	mutable std::mutex typeMutex;
	mutable std::mutex nameMutex;

	bool isPeriodScheduled(time_t rawTime) const;
	bool isSpecifiedScheduled(time_t rawTime) const;
	bool getDay_noSafe(unsigned dayIdx) const;
	unsigned getRunTime_noSafe(unsigned runTimeId) const;
	unsigned getWateringZone(time_t rawTime) const;

public:
	Program(SchedulerCallBack* callBack);
	~Program();

	unsigned getID() const { return id; }

	std::string getName() const;
	void setName(const std::string& name);

	void setType(Type type);
	Type getType() const;

	bool isScheduled(time_t rawTime) const;
	void start(time_t rawTime);
	void stop();
	bool periodic(time_t rawTime);

	// PERIOD
	void setSkipDays(unsigned skipDays) { this->skipDays = skipDays; }
	unsigned getSkipDays(void) const { return skipDays; }

	//SPECIFIED
	void enableDay(unsigned dayIdx, bool enable);
	bool isDayEnabled(unsigned dayIdx) const;

	void setRunTime(unsigned runTimeId, unsigned minutes);
	unsigned getRunTime(unsigned runTimeId) const;

	unsigned addStartTime(unsigned startTimeId, unsigned minutes);
	void deleteStartTime(unsigned startTimeId);
	void setStartTime(unsigned startTimeId, unsigned minutes);
	unsigned getStartTime(unsigned startTimeId) const;
};

#endif /* PROGRAM_H_ */
