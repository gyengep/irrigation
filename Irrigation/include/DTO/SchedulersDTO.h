#pragma once
#include <list>
#include <memory>
#include <ostream>



class SpecifiedSchedulerDTO {
	std::unique_ptr<const std::list<bool>> values;

public:
	SpecifiedSchedulerDTO();
	SpecifiedSchedulerDTO(const SpecifiedSchedulerDTO& other);
	SpecifiedSchedulerDTO(const std::list<bool>* values);

	bool operator== (const SpecifiedSchedulerDTO& other) const;

	bool hasValues() const;
	const std::list<bool>& getValues() const;
	SpecifiedSchedulerDTO& setValues(const std::list<bool>* values);

	friend std::ostream& operator<<(std::ostream& os, const SpecifiedSchedulerDTO& scheduler);
};
