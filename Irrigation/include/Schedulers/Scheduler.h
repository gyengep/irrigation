#pragma once
#include <ctime>
#include <string>


enum class SchedulerType {
	WEEKLY,
	PERIODIC,
	EVERY_DAY,
	FIXED_AMOUNT,
	FIXED_PERIOD
};

std::string to_string(SchedulerType schedulerType);


class Scheduler {
public:
	virtual ~Scheduler() = default;
	virtual void process(const std::tm& timeinfo) {}
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
	virtual unsigned getAdjustment() const { return 100; }
};
