#pragma once
#include <vector>
#include "DTO/PeriodicSchedulerDTO.h"
#include "Schedulers/Scheduler.h"



class PeriodicScheduler: public Scheduler {
	const static size_t maxPeriod = 7;

	std::vector<bool> days;
	unsigned periodStartYear, periodStartMonth, periodStartDay;

	unsigned elapsedDaysSinceEpochToPeriodStart;

	void checkIndex(size_t day) const;
	bool onProcess(const std::time_t rawtime) const;

public:
	PeriodicScheduler();
	PeriodicScheduler(PeriodicScheduler&&) = default;
	PeriodicScheduler(const PeriodicScheduler&) = default;
	PeriodicScheduler(const std::vector<bool>& days, unsigned year, unsigned month, unsigned day);
	virtual ~PeriodicScheduler();

	PeriodicScheduler& operator= (PeriodicScheduler&&) = delete;
	PeriodicScheduler& operator= (const PeriodicScheduler&) = delete;
	bool operator== (const PeriodicScheduler& other) const;

	void setPeriodStartDate(unsigned year, unsigned month, unsigned day);
	unsigned getPeriodStartYear() const { return periodStartYear; }
	unsigned getPeriodStartMonth() const { return periodStartMonth; }
	unsigned getPeriodStartDay() const { return periodStartDay; }

	void setPeriod(unsigned days);
	void enableDay(size_t day, bool enable);

	bool isDayEnabled(size_t day) const;
	unsigned getPeriod() const;

	virtual Result process(const std::time_t rawtime) override;

	PeriodicSchedulerDTO toPeriodicSchedulerDto() const;
	virtual void updateFromPeriodicSchedulerDto(const PeriodicSchedulerDTO& schedulerDTO);

	friend std::string to_string(const PeriodicScheduler& periodicScheduler);
	friend std::ostream& operator<<(std::ostream& os, const PeriodicScheduler& periodicScheduler);
};
