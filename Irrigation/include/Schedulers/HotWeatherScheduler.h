#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "DTO/HotWeatherSchedulerDto.h"
#include "Utils/OstreamInsert.h"
#include "Scheduler.h"


class HotWeatherScheduler : public Scheduler {
public:
	HotWeatherScheduler() = default;
	virtual ~HotWeatherScheduler() = default;

	///////////////////////////////////////////////////////////////////////////

	virtual void setMinTemperature(float minTemperature) = 0;
	virtual void setPeriod(const std::chrono::seconds& period) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual HotWeatherSchedulerDto toHotWeatherSchedulerDto() const = 0;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDto& schedulerDto) = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;

	///////////////////////////////////////////////////////////////////////////

	virtual nlohmann::json saveTo() const = 0;
	virtual void loadFrom(const nlohmann::json& json) = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(HotWeatherScheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<HotWeatherScheduler> HotWeatherSchedulerPtr;
typedef std::shared_ptr<const HotWeatherScheduler> ConstHotWeatherSchedulerPtr;

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerFactory {
public:
	virtual ~HotWeatherSchedulerFactory() = default;
	virtual HotWeatherSchedulerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<HotWeatherSchedulerFactory> HotWeatherSchedulerFactoryPtr;
