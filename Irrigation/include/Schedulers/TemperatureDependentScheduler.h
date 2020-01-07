#pragma once
#include <list>
#include <memory>
#include <sstream>
#include <vector>
#include "json.hpp"
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Schedulers/Scheduler.h"
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"


class TemperatureDependentScheduler : public Scheduler {
	static const std::time_t oneDayInSeconds = 24 * 60 * 60;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	int requiredAdjustmentForWholeDay;
	int remainingPercent;
	std::time_t lastRun;
	float remainingCorrection;
	int minAdjustment, maxAdjustment;
	int trim;

public:
	TemperatureDependentScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	TemperatureDependentScheduler(TemperatureDependentScheduler&&) = default;
	TemperatureDependentScheduler(const TemperatureDependentScheduler&) = default;
	TemperatureDependentScheduler(
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			float remainingCorrection,
			int minAdjustment, int maxAdjustment,
			int trim
		);
	virtual ~TemperatureDependentScheduler();

	void setRemainingCorrection(float a);
	void setMinAdjustment(unsigned minAdjustment);
	void setMaxAdjustment(unsigned maxAdjustment);
	void trimAdjustmentOver(unsigned percent);

	int getRemainingPercent() const { return remainingPercent; }
	int getRequiredPercentForNextDay(const std::time_t rawTime) const;
	int getRequiredPercentForPreviousDay(const std::time_t rawTime) const;

	virtual Result process(const std::time_t rawtime) override;

	TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO);

	friend std::string to_string(const TemperatureDependentScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const TemperatureDependentScheduler& scheduler);

	////////////////////////////////////////////////////
	virtual nlohmann::json saveTo() const;
	virtual void loadFrom(const nlohmann::json& json);

};
