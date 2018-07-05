#pragma once
#include <ctime>
#include <string>


class SpecifiedScheduler;


enum class SchedulerType {
	SPECIFIED_DAYS,
};

std::string to_string(SchedulerType schedulerType);


class SchedulerFactory {
public:
	virtual ~SchedulerFactory() = default;
	virtual SpecifiedScheduler* createSpecifiedScheduler() const;
};


class Scheduler {
public:
	virtual ~Scheduler() = default;
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
	virtual unsigned getAdjustment() const = 0;
};
