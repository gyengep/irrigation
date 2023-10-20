#pragma once
#include <chrono>
#include <memory>
#include <nlohmann/json.hpp>
#include "DTO/HotWeatherSchedulerDto.h"
#include "Temperature/TemperatureHistory.h"
#include "Schedulers/HotWeatherScheduler.h"


class HotWeatherSchedulerImpl : public HotWeatherScheduler {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	DateTime lastRun;
	std::chrono::seconds period;
	float minTemperature;

public:
	HotWeatherSchedulerImpl(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);
	HotWeatherSchedulerImpl(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::chrono::seconds& period,
			float minTemperature
		);

	virtual ~HotWeatherSchedulerImpl();

	float getMinTemperature() const;
	unsigned getPeriod() const;

	virtual void setMinTemperature(float minTemperature) override;
	virtual void setPeriod(const std::chrono::seconds& period) override;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////////////

	virtual HotWeatherSchedulerDto toHotWeatherSchedulerDto() const override;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDto& schedulerDto) override;

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
