#pragma once
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include "DTO/ProgramDTO.h"
#include "Schedulers/Scheduler.h"

class PeriodicScheduler;
class WeeklyScheduler;
class RunTimeContainer;
class StartTimeContainer;


class Program {
public:
	class Builder;

protected:
	std::string name;
	SchedulerType schedulerType;
	std::shared_ptr<PeriodicScheduler> periodicScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Program();
	Program(Program&&) = default;
	Program(const Program&);
	Program(const std::string& name, SchedulerType schedulerType,
		std::shared_ptr<PeriodicScheduler> periodicScheduler,
		std::shared_ptr<WeeklyScheduler> weeklyScheduler,
		std::shared_ptr<RunTimeContainer> runTimes,
		std::shared_ptr<StartTimeContainer> startTimes);

	virtual ~Program();

	Program& operator= (Program&&) = delete;
	Program& operator= (const Program&) = delete;
	bool operator== (const Program&) const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;

	virtual bool isScheduled(const std::tm& timeinfo) const;
	virtual const Scheduler& getCurrentScheduler() const;

	const PeriodicScheduler& getPeriodicScheduler() const { return *periodicScheduler; }
	const WeeklyScheduler& getWeeklyScheduler() const { return *weeklyScheduler; }
	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }

	PeriodicScheduler& getPeriodicScheduler() { return *periodicScheduler; }
	WeeklyScheduler& getWeeklyScheduler() { return *weeklyScheduler; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }

	ProgramDTO toProgramDto() const;
	void updateFromProgramDto(const ProgramDTO& programDTO);

	friend std::string to_string(const Program& program);
	friend std::ostream& operator<<(std::ostream& os, const Program& program);
};

class Program::Builder {
	std::string name;
	SchedulerType schedulerType;
	std::shared_ptr<PeriodicScheduler> periodicScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Builder();
	~Builder();

	Builder& setName(const std::string& name);
	Builder& setSchedulerType(SchedulerType schedulerType);
	Builder& setPeriodicScheduler(std::shared_ptr<PeriodicScheduler> periodicScheduler);
	Builder& setWeeklyScheduler(std::shared_ptr<WeeklyScheduler> weeklyScheduler);
	Builder& setRunTimeContainer(std::shared_ptr<RunTimeContainer> runTimes);
	Builder& setStartTimeContainer(std::shared_ptr<StartTimeContainer> startTimes);

	std::shared_ptr<Program> build();
};
