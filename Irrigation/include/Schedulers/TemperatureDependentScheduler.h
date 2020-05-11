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
	static const size_t logIndentation = 50;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	enum class Day { YESTERDAY, TODAY, OTHER };
	enum Day getLastRunDay(const time_t rawtime) const;
	static std::string dayToString(enum Day day);


	int remainingPercent;
	unsigned requiredPercentForToday;
	std::time_t lastRun;
	float remainingCorrection;
	std::unique_ptr<unsigned> minAdjustment;
	std::unique_ptr<unsigned> maxAdjustment;
	std::unique_ptr<unsigned> trim;

	int calculateRemainingPercentOther(const int remainingPercent, std::ostringstream& oss) const;
	int calculateRemainingPercentToday(const int remainingPercent, std::ostringstream& oss) const;
	int calculateRemainingPercentYesterday(const int remainingPercent, const std::time_t rawtime, std::ostringstream& oss) const;
	unsigned calculateRequiredPercentForToday(const int remainingPercent, const std::time_t rawtime, std::ostringstream& oss) const;
	unsigned calculateAdjustment(const unsigned requiredPercentForToday, std::ostringstream& oss) const;

public:
	TemperatureDependentScheduler(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory);
	TemperatureDependentScheduler(TemperatureDependentScheduler&&) = default;
	TemperatureDependentScheduler(const TemperatureDependentScheduler& other);
	TemperatureDependentScheduler(
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			float remainingCorrection,
			unsigned minAdjustment, unsigned maxAdjustment,
			unsigned trim
		);
	virtual ~TemperatureDependentScheduler();

	void setRemainingCorrection(float a);
	void setMinAdjustment(unsigned minAdjustment);
	void setMaxAdjustment(unsigned maxAdjustment);
	void trimAdjustmentOver(unsigned percent);

	unsigned getRemainingPercent() const { return remainingPercent; }
	unsigned getRequiredPercentForNextDay(const std::time_t rawTime, float* temp = nullptr) const;
	unsigned getRequiredPercentForPreviousDay(const std::time_t rawTime, float* temp = nullptr) const;

	virtual Result process(const std::time_t rawtime) override;

	TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO);

	friend std::string to_string(const TemperatureDependentScheduler& scheduler);
	friend std::ostream& operator<<(std::ostream& os, const TemperatureDependentScheduler& scheduler);

	////////////////////////////////////////////////////
	virtual nlohmann::json saveTo() const;
	virtual void loadFrom(const nlohmann::json& json);

};
