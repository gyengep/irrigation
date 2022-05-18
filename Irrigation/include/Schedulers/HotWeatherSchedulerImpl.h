#pragma once
#include <chrono>
#include <memory>
#include "json.hpp"
#include "DTO/HotWeatherSchedulerDTO.h"
#include "Temperature/TemperatureHistory.h"
#include "HotWeatherScheduler.h"


class HotWeatherSchedulerImpl : public HotWeatherScheduler {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	std::time_t lastRun;
	unsigned periodInSeconds;
	float minTemperature;

public:
	HotWeatherSchedulerImpl(const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	HotWeatherSchedulerImpl(HotWeatherSchedulerImpl&&) = default;
	HotWeatherSchedulerImpl(const HotWeatherSchedulerImpl&) = default;
	HotWeatherSchedulerImpl(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::chrono::seconds& period, float minTemperature
		);

	virtual ~HotWeatherSchedulerImpl();

	void setMinTemperature(float minTemperature) override;
	void setPeriod(const std::chrono::seconds& period) override;

	virtual Result process(const std::time_t rawtime) override;

	////////////////////////////////////////////////////////////

	HotWeatherSchedulerDTO toHotWeatherSchedulerDto() const override;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;

	////////////////////////////////////////////////////////////

	nlohmann::json saveTo() const override;
	void loadFrom(const nlohmann::json& json) override;
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImplFactory : public HotWeatherSchedulerFactory {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

public:
	HotWeatherSchedulerImplFactory(const std::shared_ptr<TemperatureHistory>& temperatureHistory);

	virtual ~HotWeatherSchedulerImplFactory() = default;
	virtual HotWeatherSchedulerPtr create() const override;
};
