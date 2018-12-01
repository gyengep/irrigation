#pragma once
#include <vector>
#include "DTO/PeriodicSchedulerDTO.h"
#include "Schedulers/Scheduler.h"



class PeriodicScheduler: public Scheduler {
	const static size_t minPeriod = 1;
	const static size_t maxPeriod = 7;

	std::vector<bool> days;
	unsigned adjustment;
	unsigned periodStartYear, periodStartMonth, periodStartDay;

	unsigned elapsedDaysSinceEpochToPeriodStart;

	void checkIndex(size_t day) const;

public:
	PeriodicScheduler();
	virtual ~PeriodicScheduler();

	void setPeriodStartDate(unsigned year, unsigned month, unsigned day);
	unsigned getPeriodStartYear() const { return periodStartYear; }
	unsigned getPeriodStartMonth() const { return periodStartMonth; }
	unsigned getPeriodStartDay() const { return periodStartDay; }

	void setPeriod(unsigned days);
	unsigned getPeriod() const;

	void enableDay(size_t day, bool enable);
	bool isDayEnabled(size_t day) const;

	PeriodicSchedulerDTO getPeriodicSchedulerDTO() const;
	void updateFromDTO(const PeriodicSchedulerDTO& schedulerDTO);

	virtual bool isDayScheduled(const std::tm& timeinfo) const override;

	void setAdjustment(unsigned adjustment);
	virtual unsigned getAdjustment() const override;

	friend std::string to_string(const PeriodicScheduler& periodicScheduler);
};
