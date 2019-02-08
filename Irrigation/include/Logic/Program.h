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

private:
	std::string name;
	SchedulerType schedulerType;
	std::shared_ptr<PeriodicScheduler> periodicScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

protected:
	Program();
	Program(const std::string& name, SchedulerType schedulerType,
			std::shared_ptr<PeriodicScheduler> periodicScheduler,
			std::shared_ptr<WeeklyScheduler> weeklyScheduler,
			std::shared_ptr<RunTimeContainer> runTimes,
			std::shared_ptr<StartTimeContainer> startTimes
			);

public:
	Program(Program&&) = default;
	Program(const Program&);

	virtual ~Program();

	Program& operator= (Program&&) = delete;
	Program& operator= (const Program&) = delete;
	bool operator== (const Program&) const;

	virtual bool isScheduled(const std::tm& timeinfo) const;
	virtual const Scheduler& getCurrentScheduler() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;

	const PeriodicScheduler& getPeriodicScheduler() const;
	PeriodicScheduler& getPeriodicScheduler();

	const WeeklyScheduler& getWeeklyScheduler() const;
	WeeklyScheduler& getWeeklyScheduler();

	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	RunTimeContainer& getRunTimes() { return *runTimes; }

	const StartTimeContainer& getStartTimes() const { return *startTimes; }
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
