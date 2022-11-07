#pragma once
#include "EveryDayScheduler.h"


class EveryDaySchedulerImpl : public EveryDayScheduler {
public:
	EveryDaySchedulerImpl() = default;
	virtual ~EveryDaySchedulerImpl() = default;

	virtual Scheduler::Result process(const std::time_t rawtime) override;

	////////////////////////////////////////////////////////////

	virtual EveryDaySchedulerDTO toEveryDaySchedulerDto() const override;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerImplFactory : public EveryDaySchedulerFactory {
public:
	virtual ~EveryDaySchedulerImplFactory() = default;
	virtual EveryDaySchedulerPtr create() const override;
};
