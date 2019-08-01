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
	unsigned adjustment;

public:
	Scheduler();
	virtual ~Scheduler() = default;

	void process(const std::time_t rawtime);
	bool isDayScheduled() const;
	unsigned getAdjustment() const;

	virtual unsigned onProcess(const std::time_t rawtime) = 0;
};
