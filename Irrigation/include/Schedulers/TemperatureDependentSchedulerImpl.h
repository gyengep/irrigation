#pragma once
#include <memory>
#include "json.hpp"
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"
#include "TemperatureDependentScheduler.h"


class TemperatureDependentSchedulerImpl : public TemperatureDependentScheduler {
	static const size_t logIndentation = 50;

	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	enum class Day { YESTERDAY, TODAY, OTHER };
	enum Day getLastRunDay(const LocalDateTime& localDateTime) const;
	static std::string dayToString(enum Day day);


	int remainingPercent;
	unsigned requiredPercentForToday;
	LocalDateTime lastRun;
	float remainingCorrection;
	std::unique_ptr<unsigned> minAdjustment;
	std::unique_ptr<unsigned> maxAdjustment;

	int calculateRemainingPercentOther(const int remainingPercent, std::ostringstream& oss) const;
	int calculateRemainingPercentToday(const int remainingPercent, std::ostringstream& oss) const;
	int calculateRemainingPercentYesterday(const int remainingPercent, const DateTime& dateTime, std::ostringstream& oss) const;
	unsigned calculateRequiredPercentForToday(const int remainingPercent, const DateTime& dateTime, std::ostringstream& oss) const;
	unsigned calculateAdjustment(const unsigned requiredPercentForToday, std::ostringstream& oss) const;

	static std::string optionalToString(const std::unique_ptr<unsigned>& value);

public:
	TemperatureDependentSchedulerImpl(
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);
	TemperatureDependentSchedulerImpl(
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			float remainingCorrection,
			unsigned minAdjustment, unsigned maxAdjustment
		);
	virtual ~TemperatureDependentSchedulerImpl();

	virtual void setRemainingCorrection(float a) override;
	virtual void setMinAdjustment(unsigned minAdjustment) override;
	virtual void setMaxAdjustment(unsigned maxAdjustment) override;

	unsigned getRemainingPercent() const { return remainingPercent; }
	unsigned getRequiredPercentForNextDay(const DateTime& dateTime, float* temp = nullptr) const;
	unsigned getRequiredPercentForPreviousDay(const DateTime& dateTime, float* temp = nullptr) const;

	virtual Scheduler::Result process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////

	virtual TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const override;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////

	virtual std::string toString() const override;

	////////////////////////////////////////////////////

	virtual nlohmann::json saveTo() const override;
	virtual void loadFrom(const nlohmann::json& json) override;
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerImplFactory : public TemperatureDependentSchedulerFactory {
	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

public:
	TemperatureDependentSchedulerImplFactory(
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);

	virtual ~TemperatureDependentSchedulerImplFactory() = default;
	virtual TemperatureDependentSchedulerPtr create() const override;
};
