#pragma once
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include "DTO/ProgramDTO.h"
#include "Schedulers/Scheduler.h"

class RunTimeContainer;
class StartTimeContainer;



class Program {
	// disable copy constructor and copy operator
	Program(const Program&);
	void operator= (const Program&);

	std::unique_ptr<const SchedulerFactory> schedulerFactory;

	std::string name;
	SchedulerType schedulerType;
	std::unique_ptr<SpecifiedScheduler> specifiedScheduler;
	std::unique_ptr<RunTimeContainer> runTimes;
	std::unique_ptr<StartTimeContainer> startTimes;

public:
	Program(const std::string& name = std::string());
	virtual ~Program();

	// for testing
	Program(const SchedulerFactory* schedulerFactory);

	std::string getName() const;
	virtual void setName(const std::string& name);

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;
	const Scheduler& getCurrentScheduler() const;

	const SpecifiedScheduler& getSpecifiedScheduler() const;
	SpecifiedScheduler& getSpecifiedScheduler();

	bool isScheduled(const std::time_t& rawTime) const;

	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }

	ProgramDTO getProgramDTO() const;
	void updateFromDTO(const ProgramDTO& programDTO);

	std::string toString() const;
};
