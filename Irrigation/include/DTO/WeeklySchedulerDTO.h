#pragma once
#include <list>
#include <memory>
#include <ostream>



class WeeklySchedulerDTO {
	std::unique_ptr<const std::list<bool>> values;

public:
	WeeklySchedulerDTO() = default;
	WeeklySchedulerDTO(WeeklySchedulerDTO&& other) = default;
	WeeklySchedulerDTO(const WeeklySchedulerDTO& other);
	explicit WeeklySchedulerDTO(std::list<bool>&& values);

	WeeklySchedulerDTO& operator= (WeeklySchedulerDTO&&) = default;
	WeeklySchedulerDTO& operator= (const WeeklySchedulerDTO&) = delete;
	bool operator== (const WeeklySchedulerDTO& other) const;

	bool hasValues() const;
	const std::list<bool>& getValues() const;
	WeeklySchedulerDTO& setValues(std::list<bool>&& values);

	friend std::ostream& operator<<(std::ostream& os, const WeeklySchedulerDTO& scheduler);
};
