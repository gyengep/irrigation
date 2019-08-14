#pragma once
#include <list>
#include <memory>
#include <sstream>
#include <vector>
#include "json.hpp"
#include "Schedulers/Scheduler.h"
#include "Hardware/Temperature/TemperatureForecast.h"
#include "Hardware/Temperature/TemperatureHistory.h"


class TemperatureDependentScheduler : public Scheduler {
	static const std::time_t aDayInSeconds = 24 * 60 * 60;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	int requiredAdjustmentForWholeDay;
	int remainingPercent;
	std::time_t lastRun;
	float remainingA;
	float forecastA, forecastB;
	float historyA, historyB;
	int minAdjustment;
	int maxAdjustment;
	std::unique_ptr<int> trim;

public:
	TemperatureDependentScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~TemperatureDependentScheduler();

	void setRemainingCorrection(float a);
	void setHistoryCorrection(float a, float b);
	void setForecastCorrection(float a, float b);
	void setMinAdjustment(unsigned minAdjustment);
	void setMaxAdjustment(unsigned maxAdjustment);
	void trimAdjustmentOver(unsigned percent);

	int getRemainingPercent() const { return remainingPercent; }
	int getRequiredPercentForNextDay(const std::time_t rawTime) const;
	int getRequiredPercentForPreviousDay(const std::time_t rawTime) const;

	virtual Result process(const std::time_t rawtime) override;

	virtual nlohmann::json saveTo() const;
	virtual void loadFrom(const nlohmann::json& json);

	friend std::string to_string(const TemperatureDependentScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const TemperatureDependentScheduler& scheduler);
};
