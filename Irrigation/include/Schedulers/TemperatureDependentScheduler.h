#pragma once
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "DTO/TemperatureDependentSchedulerDto.h"
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

	virtual TemperatureDependentSchedulerDto toTemperatureDependentSchedulerDto() const = 0;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDto& schedulerDto) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual nlohmann::json saveTo() const = 0;
	virtual void loadFrom(const nlohmann::json& json) = 0;
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
	virtual TemperatureDependentSchedulerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<TemperatureDependentSchedulerFactory> TemperatureDependentSchedulerFactoryPtr;
