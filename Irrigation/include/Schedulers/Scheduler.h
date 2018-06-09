#pragma once
#include <ctime>

enum class SchedulerType {
	SPECIFIED_DAYS,
};


class Scheduler {
public:
	virtual ~Scheduler() {}
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
};
