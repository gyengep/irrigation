#pragma once
#include <chrono>
#include <memory>
#include "json.hpp"
#include "DTO/HotWeatherSchedulerDTO.h"
#include "Temperature/TemperatureHistory.h"
#include "Scheduler.h"


class HotWeatherScheduler : public Scheduler {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	std::time_t lastRun;
	unsigned periodInSeconds;
	float minTemperature;

public:
	HotWeatherScheduler(const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	HotWeatherScheduler(HotWeatherScheduler&&) = default;
	HotWeatherScheduler(const HotWeatherScheduler&) = default;
	HotWeatherScheduler(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::chrono::seconds& period, float minTemperature
		);

	virtual ~HotWeatherScheduler();

	void setMinTemperature(float minTemperature);
	void setPeriod(const std::chrono::seconds& period);

	virtual Result process(const std::time_t rawtime) override;

	HotWeatherSchedulerDTO toHotWeatherSchedulerDto() const;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO);

	friend std::string to_string(const HotWeatherScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const HotWeatherScheduler& scheduler);

	////////////////////////////////////////////////////////////

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& json);
};
