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
#include <string>

#include "Containers.h"


class Scheduler;
class SpecifiedScheduler;


class Program {

	Program(const Program&);
	void operator= (const Program&);

public:

	enum SchedulerType {
		SPECIFIED_DAYS,

		LAST
	};

private:

	std::string name;
	SchedulerType schedulerType;
	std::array<Scheduler*, SchedulerType::LAST> schedulers;
	RunTimeContainer runTimes;
	StartTimeContainer startTimes;

public:
	Program();
	Program(const std::string& name);
	~Program();

	std::string getName() const;
	void setName(const std::string& name);

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType(void) const;

	const SpecifiedScheduler& getSpecifiedScheduler() const;
	SpecifiedScheduler& getSpecifiedScheduler();

	bool isScheduled(const std::time_t& rawTime) const;

	const RunTimeContainer& getRunTimes() const { return runTimes; }
	RunTimeContainer& getRunTimes() { return runTimes; }
	const StartTimeContainer& getStartTimes() const { return startTimes; }
	StartTimeContainer& getStartTimes() { return startTimes; }
};

#endif /* PROGRAM_H_ */
