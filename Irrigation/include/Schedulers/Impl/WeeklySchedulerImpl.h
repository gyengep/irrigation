#pragma once
#include <vector>
#include "DTO/WeeklySchedulerDto.h"
#include "Schedulers/WeeklyScheduler.h"


class WeeklySchedulerImpl : public WeeklyScheduler {
	static const size_t size = 7;

	std::vector<bool> days;

	void checkIndex(size_t day) const;

public:
	WeeklySchedulerImpl();
	WeeklySchedulerImpl(std::initializer_list<bool> initializer);
	virtual ~WeeklySchedulerImpl();

	virtual void enableDay(size_t day, bool enable) override;
	virtual bool isDayEnabled(size_t day) const override;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////////////

	virtual WeeklySchedulerDto toWeeklySchedulerDto() const override;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDto& schedulerDto) override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class WeeklySchedulerImplFactory : public WeeklySchedulerFactory {
public:
	virtual ~WeeklySchedulerImplFactory() = default;
	virtual WeeklySchedulerPtr create() const override;
};
