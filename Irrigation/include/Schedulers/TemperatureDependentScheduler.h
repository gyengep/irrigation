#pragma once
#include <memory>
#include <string>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Utils/OstreamInsert.h"
#include "Scheduler.h"


class TemperatureDependentScheduler : public Scheduler {
public:
	TemperatureDependentScheduler() = default;
	virtual ~TemperatureDependentScheduler() = default;

	///////////////////////////////////////////////////////////////////////////

	virtual void setRemainingCorrection(float a) = 0;
	virtual void setMinAdjustment(unsigned minAdjustment) = 0;
	virtual void setMaxAdjustment(unsigned maxAdjustment) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const = 0;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(TemperatureDependentScheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<TemperatureDependentScheduler> TemperatureDependentSchedulerPtr;
typedef std::shared_ptr<const TemperatureDependentScheduler> ConstTemperatureDependentSchedulerPtr;

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerFactory {
public:
	virtual ~TemperatureDependentSchedulerFactory() = default;
	virtual TemperatureDependentSchedulerPtr create(unsigned id) const = 0;
};
