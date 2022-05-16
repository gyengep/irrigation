#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>
#include "json.hpp"
#include "DTO/HotWeatherSchedulerDTO.h"
#include "Scheduler.h"


class HotWeatherScheduler : public Scheduler {
public:
	virtual ~HotWeatherScheduler() = default;

	virtual void setMinTemperature(float minTemperature) = 0;
	virtual void setPeriod(const std::chrono::seconds& period) = 0;

	////////////////////////////////////////////////////////////

	virtual HotWeatherSchedulerDTO toHotWeatherSchedulerDto() const = 0;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO) = 0;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const = 0;

	////////////////////////////////////////////////////////////

	virtual nlohmann::json saveTo() const = 0;
	virtual void loadFrom(const nlohmann::json& json) = 0;
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const HotWeatherScheduler& scheduler);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<HotWeatherScheduler> HotWeatherSchedulerPtr;
typedef std::shared_ptr<const HotWeatherScheduler> ConstHotWeatherSchedulerPtr;
