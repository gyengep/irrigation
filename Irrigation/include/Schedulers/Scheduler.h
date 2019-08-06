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
	struct Result {
		const bool isScheduled;
		const bool overrideAdjustment;
		const unsigned adjustment;

		Result(bool isScheduled);
		Result(unsigned adjustment);
		Result(bool isScheduled, bool overrideAdjustment, unsigned adjustment);

		bool operator== (const Result& other) const;

		friend std::ostream& operator<<(std::ostream& os, const Result& result);
	};

	virtual ~Scheduler() = default;
	virtual Result process(const std::time_t rawtime) = 0;
};
