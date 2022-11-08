#pragma once
#include <memory>
#include <string>
#include "Utils/DateTime.h"


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

	Scheduler() = default;
	virtual ~Scheduler() = default;
	virtual Result process(const LocalDateTime& localDateTime) = 0;

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<Scheduler> SchedulerPtr;
typedef std::shared_ptr<const Scheduler> ConstSchedulerPtr;

