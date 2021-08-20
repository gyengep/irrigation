#pragma once
#include <ctime>
#include <string>


enum class SchedulerType {
	EVERY_DAY,
	HOT_WEATHER,
	TEMPERATURE_DEPENDENT,
	WEEKLY,
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
