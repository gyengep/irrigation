#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentScheduler.h"


class FixedAmountScheduler : public TemperatureDependentScheduler {

public:
	FixedAmountScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedAmountScheduler();
};
