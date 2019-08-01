#pragma once
#include <vector>
#include "DTO/EveryDaySchedulerDTO.h"
#include "Schedulers/Scheduler.h"


class EveryDayScheduler : public Scheduler {
public:
	EveryDayScheduler();
	EveryDayScheduler(EveryDayScheduler&&) = default;
	EveryDayScheduler(const EveryDayScheduler&);
	virtual ~EveryDayScheduler();

	EveryDayScheduler& operator= (EveryDayScheduler&&) = delete;
	EveryDayScheduler& operator= (const EveryDayScheduler&) = delete;
	bool operator== (const EveryDayScheduler& other) const;

	virtual unsigned onProcess(const std::time_t rawtime) override;

	EveryDaySchedulerDTO toEveryDaySchedulerDto() const;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO);

	friend std::string to_string(const EveryDayScheduler& periodicScheduler);
	friend std::ostream& operator<<(std::ostream& os, const EveryDayScheduler& periodicScheduler);
};
