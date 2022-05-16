#pragma once
#include <memory>
#include <string>
#include <ostream>
#include "DTO/WeeklySchedulerDTO.h"
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
	WeeklyScheduler(WeeklyScheduler&&) = default;
	WeeklyScheduler(const WeeklyScheduler&) = default;
	virtual ~WeeklyScheduler() = default;

	WeeklyScheduler& operator= (WeeklyScheduler&&) = delete;
	WeeklyScheduler& operator= (const WeeklyScheduler&) = delete;
	bool operator== (const WeeklyScheduler& other) const;

	virtual void enableDay(size_t day, bool enable) = 0;
	virtual bool isDayEnabled(size_t day) const = 0;

	////////////////////////////////////////////////////////////

	virtual WeeklySchedulerDTO toWeeklySchedulerDto() const = 0;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO) = 0;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const WeeklyScheduler& weeklyScheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<WeeklyScheduler> WeeklySchedulerPtr;
typedef std::shared_ptr<const WeeklyScheduler> ConstWeeklySchedulerPtr;
