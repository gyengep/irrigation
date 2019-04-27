#pragma once
#include <ctime>
#include <string>


enum class SchedulerType {
	WEEKLY,
	PERIODIC
};

std::string to_string(SchedulerType schedulerType);


class Scheduler {
public:
	virtual ~Scheduler() = default;
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
};
