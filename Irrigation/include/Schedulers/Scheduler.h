#pragma once
#include <ctime>
#include <memory>
#include <string>

class PeriodicScheduler;
class WeeklyScheduler;


enum class SchedulerType {
	WEEKLY,
	PERIODIC
};

std::string to_string(SchedulerType schedulerType);


class Scheduler {
public:
	virtual ~Scheduler() = default;
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
	virtual unsigned getAdjustment() const = 0;
};

class SchedulerFactory {
public:
	virtual ~SchedulerFactory() = default;
	virtual std::unique_ptr<WeeklyScheduler> createWeeklyScheduler() const;
	virtual std::unique_ptr<PeriodicScheduler> createPeriodicScheduler() const;
};
