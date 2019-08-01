#pragma once
#include <list>
#include <memory>
#include <vector>
#include "json.hpp"
#include "Schedulers/Scheduler.h"
#include "Hardware/Temperature/TemperatureForecast.h"
#include "Hardware/Temperature/TemperatureHistory.h"


class TemperatureDependentScheduler : public Scheduler {
	static const std::time_t aDayInSeconds;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	std::vector<std::pair<float, int>> temperatureAndPercents;

	int remainingPercent;
	int adjustment;
	std::time_t lastRun;

	virtual int onCalculateAdjustment(const std::time_t rawTime);

public:
	TemperatureDependentScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~TemperatureDependentScheduler();

	void setTemperatureAndPercents(const std::vector<std::pair<float, int>>& temperatureAndPercents);
	int getRequiredPercentFromTemperature(float temperature) const;

	int getRemainingPercent() const { return remainingPercent; }
	int getRequiredPercentForNextDay(const std::time_t rawTime) const;
	int getRequiredPercentForPreviousDay(const std::time_t rawTime) const;

	virtual Result process(const std::time_t rawtime) override;

	virtual nlohmann::json saveTo() const;
	virtual void loadFrom(const nlohmann::json& json);
};


class FixedAmountScheduler : public TemperatureDependentScheduler {

	virtual int onCalculateAdjustment(const std::time_t rawTime) override;

public:
	FixedAmountScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedAmountScheduler();
};


class FixedPeriodScheduler : public TemperatureDependentScheduler {

	virtual int onCalculateAdjustment(const std::time_t rawTime) override;

public:
	FixedPeriodScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedPeriodScheduler();
};
