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
	class Result;

	virtual ~Scheduler() = default;
	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) = 0;

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class Scheduler::Result {
	const unsigned adjustment;

public:
	Result(bool isScheduled);
	Result(unsigned adjustment);

	bool isSkipped() const;
	unsigned getAdjustment() const;

	bool operator== (const Result& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Result& result);
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<Scheduler> SchedulerPtr;
typedef std::shared_ptr<const Scheduler> ConstSchedulerPtr;

