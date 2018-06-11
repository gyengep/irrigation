#pragma once
#include <ctime>


class SpecifiedScheduler;


enum class SchedulerType {
	SPECIFIED_DAYS,
};


class SchedulerFactory {
public:
	virtual ~SchedulerFactory() = default;
	virtual SpecifiedScheduler* createSpecifiedScheduler() const;
};


class Scheduler {
public:
	virtual ~Scheduler() = default;
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
};
