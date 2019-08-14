#pragma once
#include <chrono>
#include <memory>
#include "json.hpp"
#include "Hardware/Temperature/TemperatureHistory.h"
#include "Scheduler.h"


class HotWeatherScheduler : public Scheduler {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	std::time_t lastRun;
	unsigned periodInSeconds;
	float minTemperature;

public:
	HotWeatherScheduler(const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	virtual ~HotWeatherScheduler();

	void setMinTemperature(float minTemperature);
	void setPeriod(const std::chrono::seconds& period);

	virtual Result process(const std::time_t rawtime) override;

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& json);

	friend std::string to_string(const HotWeatherScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const HotWeatherScheduler& scheduler);
};
