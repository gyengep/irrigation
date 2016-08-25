/*
 * Program.h
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <array>
//#include <climits>
#include <ctime>
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "common.h"


class DayScheduler;

class Program {

	static const unsigned ZONE_COUNT = 6;

public:

	typedef std::array<std::pair<IdType, unsigned>, ZONE_COUNT> RunTimes;
	typedef std::list<std::pair<IdType, unsigned>> StartTimes;


	enum SchedulerType {
		SPECIFIED_DAYS,

		LAST
	};

private:

	// Name
	std::string name;

	// Watering day
	SchedulerType schedulerType;
	std::array<DayScheduler*, SchedulerType::LAST> schedulers;

	// Runtime
	RunTimes runTimes;

	// StartTime
	IdType nextStartTimeId;
	StartTimes startTimes;

public:
	Program();
	~Program();

	std::string getName() const;
	void setName(const std::string& name);

	bool isScheduled(std::time_t rawTime) const;

	// Runtime
	const RunTimes& getRunTimes() const;
	void setRunTime(IdType id, unsigned minutes);
	unsigned getRunTime(IdType id) const;

	// StartTime
	const StartTimes& getStartTimes() const;
	IdType addStartTime(unsigned minutes);
	void deleteStartTime(IdType id);
	void setStartTime(IdType id, unsigned minutes);
	unsigned getStartTime(IdType id) const;
};

#endif /* PROGRAM_H_ */
