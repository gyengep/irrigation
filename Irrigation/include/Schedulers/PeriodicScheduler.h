#pragma once
#include <vector>
#include "DTO/PeriodicSchedulerDTO.h"
#include "Schedulers/Scheduler.h"



class PeriodicScheduler: public Scheduler {
	const static size_t maxPeriod = 7;

	std::vector<bool> days;
	unsigned adjustment;
	unsigned periodStartYear, periodStartMonth, periodStartDay;

	unsigned elapsedDaysSinceEpochToPeriodStart;

	void checkIndex(size_t day) const;

public:
	PeriodicScheduler();
	PeriodicScheduler(PeriodicScheduler&&) = default;
	PeriodicScheduler(const PeriodicScheduler&);
	virtual ~PeriodicScheduler();

	PeriodicScheduler& operator= (PeriodicScheduler&&) = delete;
	PeriodicScheduler& operator= (const PeriodicScheduler&) = delete;
	bool operator== (const PeriodicScheduler& other) const;

	void setPeriodStartDate(unsigned year, unsigned month, unsigned day);
	unsigned getPeriodStartYear() const { return periodStartYear; }
	unsigned getPeriodStartMonth() const { return periodStartMonth; }
	unsigned getPeriodStartDay() const { return periodStartDay; }

	void setAdjustment(unsigned adjustment);
	void setPeriod(unsigned days);
	void enableDay(size_t day, bool enable);

	bool isDayEnabled(size_t day) const;
	unsigned getPeriod() const;

	virtual bool isDayScheduled(const std::tm& timeinfo) const override;
	virtual unsigned getAdjustment() const override;

	PeriodicSchedulerDTO toPeriodicSchedulerDto() const;
	void updateFromPeriodicSchedulerDto(const PeriodicSchedulerDTO& schedulerDTO);

	friend std::string to_string(const PeriodicScheduler& periodicScheduler);
	friend std::ostream& operator<<(std::ostream& os, const PeriodicScheduler& periodicScheduler);
};
