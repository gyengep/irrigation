#pragma once
#include <memory>
#include <string>
#include "DTO/EveryDaySchedulerDTO.h"
#include "Utils/OstreamInsert.h"
#include "Scheduler.h"


class EveryDayScheduler : public Scheduler {
public:
	EveryDayScheduler() = default;
	virtual ~EveryDayScheduler() = default;

	///////////////////////////////////////////////////////////////////////////

	virtual EveryDaySchedulerDTO toEveryDaySchedulerDto() const = 0;
	virtual void updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(EveryDayScheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<EveryDayScheduler> EveryDaySchedulerPtr;
typedef std::shared_ptr<const EveryDayScheduler> ConstEveryDaySchedulerPtr;

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerFactory {
public:
	virtual ~EveryDaySchedulerFactory() = default;
	virtual EveryDaySchedulerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<EveryDaySchedulerFactory> EveryDaySchedulerFactoryPtr;
