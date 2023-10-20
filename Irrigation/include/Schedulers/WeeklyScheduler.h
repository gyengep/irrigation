#pragma once
#include <memory>
#include <string>
#include "DTO/WeeklySchedulerDto.h"
#include "Utils/OstreamInsert.h"
#include "Scheduler.h"


class WeeklyScheduler : public Scheduler {
public:

	static const size_t count = 7;

	enum {
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY,
		SUNDAY
	};

	WeeklyScheduler() = default;
	virtual ~WeeklyScheduler() = default;

	///////////////////////////////////////////////////////////////////////////

	virtual void enableDay(size_t day, bool enable) = 0;
	virtual bool isDayEnabled(size_t day) const = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual WeeklySchedulerDto toWeeklySchedulerDto() const = 0;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDto& schedulerDto) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(WeeklyScheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<WeeklyScheduler> WeeklySchedulerPtr;
typedef std::shared_ptr<const WeeklyScheduler> ConstWeeklySchedulerPtr;

///////////////////////////////////////////////////////////////////////////////

class WeeklySchedulerFactory {
public:
	virtual ~WeeklySchedulerFactory() = default;
	virtual WeeklySchedulerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<WeeklySchedulerFactory> WeeklySchedulerFactoryPtr;
