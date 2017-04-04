#pragma once

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
	std::unique_ptr<RunTimeContainer> runTimes;
	std::unique_ptr<StartTimeContainer> startTimes;

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

	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }
};
