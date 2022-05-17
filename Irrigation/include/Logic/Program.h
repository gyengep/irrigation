#pragma once
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include "json.hpp"
#include "DTO/ProgramDTO.h"
#include "Schedulers/Scheduler.h"
#include "Utils/OstreamInsert.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"

class EveryDayScheduler;
class HotWeatherScheduler;
class TemperatureDependentScheduler;
class WeeklyScheduler;


class ScheduledResult {
	const bool scheduled;
	const unsigned adjustment;

public:
	ScheduledResult(bool scheduled, unsigned adjustment);

	bool isScheduled() const;
	unsigned getAdjustment() const;

	bool operator==(const ScheduledResult& other) const;
};


class Program {
public:
	class Builder;

protected:
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<Scheduler> currentScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Program();
	Program(Program&&) = default;
	Program(const Program&) = delete;
	Program(bool enabled, const std::string& name,
		unsigned adjustment,
		SchedulerType schedulerType,
		const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
		const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
		const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
		const std::shared_ptr<WeeklyScheduler>& weeklyScheduler,
		const std::shared_ptr<RunTimeContainer>& runTimes,
		const std::shared_ptr<StartTimeContainer>& startTimes);
	Program(const ProgramDTO& programDTO);

	virtual ~Program();

	Program& operator= (Program&&) = delete;
	Program& operator= (const Program&) = delete;
	bool operator== (const Program&) const;

	void setEnabled(bool enabled);
	bool isEnabled() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setAdjustment(unsigned adjustment);
	unsigned getAdjustment() const;

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;

	virtual std::unique_ptr<ScheduledResult> isScheduled(const std::time_t rawtime);
//	virtual const Scheduler& getCurrentScheduler() const { return *currentScheduler; }
	virtual Scheduler& getCurrentScheduler() { return *currentScheduler; }

	const EveryDayScheduler& getEveryDayScheduler() const { return *everyDayScheduler; }
	const HotWeatherScheduler& getHotWeatherScheduler() const { return *hotWeatherScheduler; }
	const TemperatureDependentScheduler& getTemperatureDependentScheduler() const { return *temperatureDependentScheduler; }
	const WeeklyScheduler& getWeeklyScheduler() const { return *weeklyScheduler; }
	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }

	EveryDayScheduler& getEveryDayScheduler() { return *everyDayScheduler; }
	HotWeatherScheduler& getHotWeatherScheduler() { return *hotWeatherScheduler; }
	TemperatureDependentScheduler& getTemperatureDependentScheduler() { return *temperatureDependentScheduler; }
	WeeklyScheduler& getWeeklyScheduler() { return *weeklyScheduler; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }

	ProgramDTO toProgramDto() const;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO);

	std::string toString() const;

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& values);
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(Program);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<Program> ProgramPtr;

class Program::Builder {
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<RunTimeContainer> runTimes;
	std::shared_ptr<StartTimeContainer> startTimes;

public:
	Builder();
	~Builder();

	Builder& setEnabled(bool enabled);
	Builder& setName(const std::string& name);
	Builder& setAdjustment(unsigned adjustment);
	Builder& setSchedulerType(SchedulerType schedulerType);
	Builder& setEveryDayScheduler(const std::shared_ptr<EveryDayScheduler>& everyDayScheduler);
	Builder& setHotWeatherScheduler(const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler);
	Builder& setTemperatureDependentScheduler(const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler);
	Builder& setWeeklyScheduler(const std::shared_ptr<WeeklyScheduler>& weeklyScheduler);
	Builder& setRunTimeContainer(const std::shared_ptr<RunTimeContainer>& runTimes);
	Builder& setStartTimeContainer(const std::shared_ptr<StartTimeContainer>& startTimes);

	ProgramPtr build();
};
