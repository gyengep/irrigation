#pragma once
#include <ctime>
#include <vector>
#include "DTO/WeeklySchedulerDTO.h"
#include "WeeklyScheduler.h"


class WeeklySchedulerImpl : public WeeklyScheduler {
	static const size_t size = 7;

	std::vector<bool> days;

	void checkIndex(size_t day) const;
	bool onProcess(const std::time_t rawtime) const;

public:
	WeeklySchedulerImpl();
	WeeklySchedulerImpl(WeeklySchedulerImpl&&) = default;
	WeeklySchedulerImpl(const WeeklySchedulerImpl&) = default;
	WeeklySchedulerImpl(std::initializer_list<bool> initializer);
	virtual ~WeeklySchedulerImpl();

	virtual void enableDay(size_t day, bool enable) override;
	virtual bool isDayEnabled(size_t day) const override;

	virtual Scheduler::Result process(const std::time_t rawtime) override;

	////////////////////////////////////////////////////////////

	virtual WeeklySchedulerDTO toWeeklySchedulerDto() const override;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};
