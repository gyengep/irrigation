#pragma once
#include <list>
#include <memory>
#include <ostream>



class WeeklySchedulerDTO {
	std::unique_ptr<unsigned> adjustment;
	std::unique_ptr<const std::list<bool>> values;

public:
	WeeklySchedulerDTO() = default;
	WeeklySchedulerDTO(WeeklySchedulerDTO&& other) = default;
	WeeklySchedulerDTO(const WeeklySchedulerDTO& other);
	WeeklySchedulerDTO(unsigned adjustment, std::list<bool>&& values);

	WeeklySchedulerDTO& operator= (WeeklySchedulerDTO&&) = default;
	WeeklySchedulerDTO& operator= (const WeeklySchedulerDTO&) = delete;
	bool operator== (const WeeklySchedulerDTO& other) const;

	bool hasAdjustment() const;
	bool hasValues() const;

	unsigned getAdjustment() const;
	const std::list<bool>& getValues() const;

	WeeklySchedulerDTO& setAdjustment(unsigned adjustment);
	WeeklySchedulerDTO& setValues(std::list<bool>&& values);

	friend std::ostream& operator<<(std::ostream& os, const WeeklySchedulerDTO& scheduler);
};
