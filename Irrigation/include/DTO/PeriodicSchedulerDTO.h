#pragma once
#include <memory>
#include <list>


class PeriodicSchedulerDTO {
	std::unique_ptr<unsigned> adjustment;
	std::unique_ptr<const std::list<bool>> values;
	std::unique_ptr<unsigned> periodStartYear, periodStartMonth, periodStartDay;

public:
	PeriodicSchedulerDTO() = default;
	PeriodicSchedulerDTO(PeriodicSchedulerDTO&& other) = default;
	PeriodicSchedulerDTO(const PeriodicSchedulerDTO& other);
	PeriodicSchedulerDTO(unsigned adjustment, std::list<bool>&& values,
			unsigned periodStartYear, unsigned periodStartMonth, unsigned periodStartDay);

	PeriodicSchedulerDTO& operator= (PeriodicSchedulerDTO&&) = default;
	PeriodicSchedulerDTO& operator= (const PeriodicSchedulerDTO&) = delete;
	bool operator== (const PeriodicSchedulerDTO& other) const;

	bool hasAdjustment() const;
	bool hasValues() const;
	bool hasPeriodStartYear() const;
	bool hasPeriodStartMonth() const;
	bool hasPeriodStartDay() const;

	unsigned getAdjustment() const;
	const std::list<bool>& getValues() const;
	unsigned getPeriodStartYear() const;
	unsigned getPeriodStartMonth() const;
	unsigned getPeriodStartDay() const;

	PeriodicSchedulerDTO& setAdjustment(unsigned adjustment);
	PeriodicSchedulerDTO& setValues(std::list<bool>&& values);
	PeriodicSchedulerDTO& setPeriodStartYear(unsigned year);
	PeriodicSchedulerDTO& setPeriodStartMonth(unsigned month);
	PeriodicSchedulerDTO& setPeriodStartDay(unsigned day);

	friend std::ostream& operator<<(std::ostream& os, const PeriodicSchedulerDTO& scheduler);
};
