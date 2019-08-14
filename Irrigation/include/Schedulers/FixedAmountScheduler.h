#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentScheduler.h"


class FixedAmountScheduler : public TemperatureDependentScheduler {

public:
	FixedAmountScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedAmountScheduler();

	friend std::string to_string(const FixedAmountScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const FixedAmountScheduler& scheduler);
};
