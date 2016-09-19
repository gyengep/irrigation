/*
 * Program.h
 *
 *  Created on: 2016. máj. 19.
 *      Author: Rendszergazda
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <array>
#include <ctime>
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "common.h"
#include "Containers.h"


class DayScheduler;
class Program_SetName_Test;
class Program_AddStartTime_Test;
class Program_DeleteStartTime_Test;
class Program_SetStartTime_Test;
class Program_GetStartTime_Test;

class Program {

	friend Program_SetName_Test;
	friend Program_AddStartTime_Test;
	friend Program_DeleteStartTime_Test;
	friend Program_SetStartTime_Test;
	friend Program_GetStartTime_Test;

public:

	typedef RunTimeContainer::RunTimes RunTimes;
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
	RunTimeContainer runTimeContainer;

	// StartTime
	IdType nextStartTimeId;
	StartTimes startTimes;

public:
	Program();
	~Program();

	std::string getName() const;
	void setName(const std::string& name);

	bool isScheduled(const std::time_t& rawTime) const;

	// Runtime
	const RunTimeContainer& runTimes() const { return runTimeContainer; }
	RunTimeContainer& runTimes() { return runTimeContainer; }

	// StartTime
	const StartTimes& getStartTimes() const;
	IdType addStartTime(unsigned minutes);
	void deleteStartTime(IdType id);
	void setStartTime(IdType id, unsigned minutes);
	unsigned getStartTime(IdType id) const;
};

#endif /* PROGRAM_H_ */
