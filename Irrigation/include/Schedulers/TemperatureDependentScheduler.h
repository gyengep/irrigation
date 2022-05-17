#pragma once
#include <memory>
#include <string>
#include "json.hpp"
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Utils/OstreamInsert.h"
#include "Scheduler.h"


class TemperatureDependentScheduler : public Scheduler {
public:
	virtual ~TemperatureDependentScheduler() = default;

	virtual void setRemainingCorrection(float a) = 0;
	virtual void setMinAdjustment(unsigned minAdjustment) = 0;
	virtual void setMaxAdjustment(unsigned maxAdjustment) = 0;
	virtual void trimAdjustmentOver(unsigned percent) = 0;

	////////////////////////////////////////////////////////////

	virtual TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const = 0;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) = 0;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;

	////////////////////////////////////////////////////

	virtual nlohmann::json saveTo() const = 0;
	virtual void loadFrom(const nlohmann::json& json) = 0;
};

////////////////////////////////////////////////////

OSTREAM_INSERT(TemperatureDependentScheduler);
