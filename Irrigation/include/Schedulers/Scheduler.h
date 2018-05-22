#pragma once
#include <ctime>


class Scheduler {
public:
	virtual ~Scheduler() {}
	virtual bool isDayScheduled(const std::tm& timeinfo) const = 0;
};
