#pragma once
#include "EveryDayScheduler.h"


class EveryDaySchedulerImpl : public EveryDayScheduler {
public:
	EveryDaySchedulerImpl() = default;
	virtual ~EveryDaySchedulerImpl() = default;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////////////

	virtual EveryDaySchedulerDTO toEveryDaySchedulerDto() const override;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual void createPersistedData() override;
	virtual void deletePersistedData() override;
	virtual void loadPersistedData() override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerImplFactory : public EveryDaySchedulerFactory {
public:
	virtual ~EveryDaySchedulerImplFactory() = default;
	virtual EveryDaySchedulerPtr create() const override;
};
