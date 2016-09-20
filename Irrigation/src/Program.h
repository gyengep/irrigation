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

// TODO "common.h"-kat cpp be athelyezni
#include "common.h"
#include "Containers.h"


class Scheduler;
class SpecifiedScheduler;

class Program {
public:

	typedef RunTimeContainer::RunTimes RunTimes;
	typedef StartTimeContainer::StartTimes StartTimes;


	enum SchedulerType {
		SPECIFIED_DAYS,

		LAST
	};

private:

	// Name
	std::string name;

	// Watering day
	SchedulerType schedulerType;
	std::array<Scheduler*, SchedulerType::LAST> schedulers;

	RunTimeContainer runTimeContainer;
	StartTimeContainer startTimeContainer;

public:
	Program();
	~Program();

	std::string getName() const;
	void setName(const std::string& name);

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType(void) const;

	const SpecifiedScheduler& getSpecifiedScheduler() const;
	SpecifiedScheduler& getSpecifiedScheduler();

	bool isScheduled(const std::time_t& rawTime) const;

	const RunTimeContainer& runTimes() const { return runTimeContainer; }
	RunTimeContainer& runTimes() { return runTimeContainer; }
	const StartTimeContainer& startTimes() const { return startTimeContainer; }
	StartTimeContainer& startTimes() { return startTimeContainer; }
};

#endif /* PROGRAM_H_ */
