#pragma once
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include "json.hpp"
#include "DTO/ProgramDTO.h"
#include "Schedulers/Scheduler.h"

class PeriodicScheduler;
class WeeklyScheduler;
class EveryDayScheduler;
class FixedAmountScheduler;
class FixedPeriodScheduler;
class RunTimeContainer;
class StartTimeContainer;


class Program {
public:
	class Builder;

protected:
	bool disabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<PeriodicScheduler> periodicScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<FixedAmountScheduler> fixedAmountScheduler;
	std::shared_ptr<FixedPeriodScheduler> fixedPeriodScheduler;
	std::shared_ptr<Scheduler> currentScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Program();
	Program(Program&&) = default;
	Program(const Program&);
	Program(bool disabled, const std::string& name,
		unsigned adjustment,
		SchedulerType schedulerType,
		std::shared_ptr<PeriodicScheduler> periodicScheduler,
		std::shared_ptr<WeeklyScheduler> weeklyScheduler,
		std::shared_ptr<EveryDayScheduler> everyDayScheduler,
		std::shared_ptr<RunTimeContainer> runTimes,
		std::shared_ptr<StartTimeContainer> startTimes);
	Program(bool disabled, const std::string& name,
		unsigned adjustment,
		SchedulerType schedulerType,
		std::shared_ptr<PeriodicScheduler> periodicScheduler,
		std::shared_ptr<WeeklyScheduler> weeklyScheduler,
		std::shared_ptr<EveryDayScheduler> everyDayScheduler,
		std::shared_ptr<FixedAmountScheduler> fixedAmountScheduler,
		std::shared_ptr<FixedPeriodScheduler> fixedPeriodScheduler,
		std::shared_ptr<RunTimeContainer> runTimes,
		std::shared_ptr<StartTimeContainer> startTimes);
	Program(const ProgramDTO& programDTO);

	virtual ~Program();

	Program& operator= (Program&&) = delete;
	Program& operator= (const Program&) = delete;
	bool operator== (const Program&) const;

	void setDisabled(bool disabled);
	bool isDisabled() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setAdjustment(unsigned adjustment);
	unsigned getAdjustment() const;

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;

	virtual bool isScheduled(const std::tm& timeinfo);
	virtual const Scheduler& getCurrentScheduler() const { return *currentScheduler; }
	virtual Scheduler& getCurrentScheduler() { return *currentScheduler; }

	const PeriodicScheduler& getPeriodicScheduler() const { return *periodicScheduler; }
	const WeeklyScheduler& getWeeklyScheduler() const { return *weeklyScheduler; }
	const EveryDayScheduler& getEveryDayScheduler() const { return *everyDayScheduler; }
	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }

	PeriodicScheduler& getPeriodicScheduler() { return *periodicScheduler; }
	WeeklyScheduler& getWeeklyScheduler() { return *weeklyScheduler; }
	EveryDayScheduler& getEveryDayScheduler() { return *everyDayScheduler; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }

	ProgramDTO toProgramDto() const;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO);

	friend std::string to_string(const Program& program);
	friend std::ostream& operator<<(std::ostream& os, const Program& program);

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& values);
};

class Program::Builder {
	bool disabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<PeriodicScheduler> periodicScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<FixedAmountScheduler> fixedAmountScheduler;
	std::shared_ptr<FixedPeriodScheduler> fixedPeriodScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Builder();
	~Builder();

	Builder& setDisabled(bool disabled);
	Builder& setName(const std::string& name);
	Builder& setAdjustment(unsigned adjustment);
	Builder& setSchedulerType(SchedulerType schedulerType);
	Builder& setPeriodicScheduler(std::shared_ptr<PeriodicScheduler> periodicScheduler);
	Builder& setWeeklyScheduler(std::shared_ptr<WeeklyScheduler> weeklyScheduler);
	Builder& setEveryDayScheduler(std::shared_ptr<EveryDayScheduler> everyDayScheduler);
	Builder& setFixedAmountScheduler(std::shared_ptr<FixedAmountScheduler> fixedAmountScheduler);
	Builder& setFixedPeriodScheduler(std::shared_ptr<FixedPeriodScheduler> fixedPeriodScheduler);
	Builder& setRunTimeContainer(std::shared_ptr<RunTimeContainer> runTimes);
	Builder& setStartTimeContainer(std::shared_ptr<StartTimeContainer> startTimes);

	std::shared_ptr<Program> build();
};
