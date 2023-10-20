#pragma once
#include "Schedulers/EveryDayScheduler.h"


class EveryDaySchedulerImpl : public EveryDayScheduler {
public:
	EveryDaySchedulerImpl() = default;
	virtual ~EveryDaySchedulerImpl() = default;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////////////

	virtual EveryDaySchedulerDto toEveryDaySchedulerDto() const override;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDto& schedulerDto) override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerImplFactory : public EveryDaySchedulerFactory {
public:
	virtual ~EveryDaySchedulerImplFactory() = default;
	virtual EveryDaySchedulerPtr create() const override;
};
