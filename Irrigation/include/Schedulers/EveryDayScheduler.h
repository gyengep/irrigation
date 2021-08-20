#pragma once
#include <vector>
#include "DTO/EveryDaySchedulerDTO.h"
#include "Schedulers/Scheduler.h"


class EveryDayScheduler : public Scheduler {
public:
	EveryDayScheduler() = default;
	EveryDayScheduler(EveryDayScheduler&&) = default;
	EveryDayScheduler(const EveryDayScheduler&) = default;
	virtual ~EveryDayScheduler() = default;

	EveryDayScheduler& operator= (EveryDayScheduler&&) = delete;
	EveryDayScheduler& operator= (const EveryDayScheduler&) = delete;
	bool operator== (const EveryDayScheduler& other) const;

	virtual Result process(const std::time_t rawtime) override;

	EveryDaySchedulerDTO toEveryDaySchedulerDto() const;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO);

	friend std::string to_string(const EveryDayScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const EveryDayScheduler& scheduler);
};
