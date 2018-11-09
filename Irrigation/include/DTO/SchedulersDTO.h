#pragma once
#include <list>
#include <memory>
#include <ostream>



class SpecifiedSchedulerDTO {
	std::unique_ptr<unsigned> adjustment;
	std::unique_ptr<const std::list<bool>> values;

public:
	SpecifiedSchedulerDTO() = default;
	SpecifiedSchedulerDTO(SpecifiedSchedulerDTO&& other) = default;
	SpecifiedSchedulerDTO(const SpecifiedSchedulerDTO& other);
	SpecifiedSchedulerDTO(unsigned adjustment, std::list<bool>&& values);

	SpecifiedSchedulerDTO& operator= (SpecifiedSchedulerDTO&&) = delete;
	SpecifiedSchedulerDTO& operator= (const SpecifiedSchedulerDTO&) = delete;
	bool operator== (const SpecifiedSchedulerDTO& other) const;

	bool hasAdjustment() const;
	bool hasValues() const;

	unsigned getAdjustment() const;
	const std::list<bool>& getValues() const;

	SpecifiedSchedulerDTO& setAdjustment(unsigned adjustment);
	SpecifiedSchedulerDTO& setValues(std::list<bool>&& values);

	friend std::ostream& operator<<(std::ostream& os, const SpecifiedSchedulerDTO& scheduler);
};
