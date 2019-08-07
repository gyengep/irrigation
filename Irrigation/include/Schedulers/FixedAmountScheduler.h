#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentScheduler.h"


class FixedAmountScheduler : public TemperatureDependentScheduler {

	virtual int onCalculateAdjustment(const std::time_t rawTime) override;

public:
	FixedAmountScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedAmountScheduler();
};
