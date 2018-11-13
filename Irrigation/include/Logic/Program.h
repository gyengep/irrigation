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
	SchedulerFactory schedulerFactory;

	std::string name;
	SchedulerType schedulerType;
	std::unique_ptr<WeeklyScheduler> weeklyScheduler;
	std::unique_ptr<RunTimeContainer> runTimes;
	std::unique_ptr<StartTimeContainer> startTimes;

public:
	Program();
	Program(Program&&) = default;
	Program(const Program&) = delete;
	virtual ~Program();

	Program& operator= (Program&&) = default;
	Program& operator= (const Program&) = delete;

	virtual bool isScheduled(const std::time_t& rawTime) const;
	virtual const Scheduler& getCurrentScheduler() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;

	const WeeklyScheduler& getWeeklyScheduler() const;
	WeeklyScheduler& getWeeklyScheduler();

	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	RunTimeContainer& getRunTimes() { return *runTimes; }

	const StartTimeContainer& getStartTimes() const { return *startTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }

	ProgramDTO getProgramDTO() const;
	void updateFromDTO(const ProgramDTO& programDTO);

	friend std::string to_string(const Program& program);
};
