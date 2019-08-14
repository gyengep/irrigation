#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentScheduler.h"


class FixedPeriodScheduler : public TemperatureDependentScheduler {

public:
	FixedPeriodScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedPeriodScheduler();

	friend std::string to_string(const FixedPeriodScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const FixedPeriodScheduler& scheduler);
};
