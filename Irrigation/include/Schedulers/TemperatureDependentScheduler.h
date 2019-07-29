#pragma once
#include <list>
#include <memory>
#include <vector>
#include "json.hpp"
#include "Schedulers/Scheduler.h"
#include "Hardware/Temperature/TemperatureForecast.h"
#include "Hardware/Temperature/TemperatureHistory.h"

namespace TemperatureDependentScheduler {

class BaseScheduler : public Scheduler {
	struct findKey {
		const std::string& key;

		findKey(const std::string& key) : key(key) {}

		bool operator() (const std::pair<std::string, std::string>& a) {
			return (a.first == key);
		}
	};

	static const std::time_t aDayInSeconds;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	std::vector<std::pair<float, int>> temperatureAndPercents;

	int remainingPercent;
	int maxAdjustment;
	int adjustment;
	bool first;

public:
	BaseScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~BaseScheduler();

	void setTemperatureAndPercents(const std::vector<std::pair<float, int>>& temperatureAndPercents);
	int getRequiredPercentFromTemperature(float temperature) const;

	int getRemainingPercent() const { return remainingPercent; }
	int getRequiredPercentForNextDay(const std::time_t = std::time(nullptr)) const;
	int getRequiredPercentForPreviousDay(const std::time_t = std::time(nullptr)) const;

	virtual void process(const std::tm& timeinfo) override;
	virtual bool isDayScheduled(const std::tm& timeinfo) const override;
	virtual unsigned getAdjustment() const override;

	virtual int calculateAdjustment() { return 0; }

	virtual nlohmann::json saveTo() const;
	virtual void loadFrom(const nlohmann::json& json);
};


class FixedAmountScheduler : public BaseScheduler {

public:
	FixedAmountScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedAmountScheduler();

	virtual int calculateAdjustment() override;
};

class FixedPeriodScheduler : public BaseScheduler {
public:
	FixedPeriodScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~FixedPeriodScheduler();

	virtual int calculateAdjustment() override;
};

}
