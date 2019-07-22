#pragma once
#include <ostream>


class EveryDaySchedulerDTO {
public:
	EveryDaySchedulerDTO() = default;
	EveryDaySchedulerDTO(EveryDaySchedulerDTO&& other) = default;
	EveryDaySchedulerDTO(const EveryDaySchedulerDTO& other) = default;

	EveryDaySchedulerDTO& operator= (EveryDaySchedulerDTO&&) = default;
	EveryDaySchedulerDTO& operator= (const EveryDaySchedulerDTO&) = delete;
	bool operator== (const EveryDaySchedulerDTO& other) const;

	friend std::ostream& operator<<(std::ostream& os, const EveryDaySchedulerDTO& scheduler);
};
