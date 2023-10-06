#pragma once
#include <memory>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Persistence/TemperatureDependentSchedulerRepository.h"
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"
#include "TemperatureDependentScheduler.h"


class TemperatureDependentSchedulerImpl : public TemperatureDependentScheduler {
public:
	class PersistedData;

private:
	static const size_t logIndentation = 50;

	const std::shared_ptr<PersistedData> persistedData;
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
			const std::shared_ptr<PersistedData>& persistedData,
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);
	TemperatureDependentSchedulerImpl(
			const std::shared_ptr<PersistedData>& persistedData,
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			float remainingCorrection,
			unsigned minAdjustment, unsigned maxAdjustment
		);
	virtual ~TemperatureDependentSchedulerImpl();

	virtual void setRemainingCorrection(float a) override;
	virtual void setMinAdjustment(unsigned minAdjustment) override;
	virtual void setMaxAdjustment(unsigned maxAdjustment) override;

	LocalDateTime getLastRun() const     { return lastRun; }
	unsigned getRemainingPercent() const { return remainingPercent; }
	unsigned getRequiredPercentForNextDay(const DateTime& dateTime, float* temp = nullptr) const;
	unsigned getRequiredPercentForPreviousDay(const DateTime& dateTime, float* temp = nullptr) const;

	float getRemainingCorrection() const;
	unsigned getMinAdjustment() const;
	unsigned getMaxAdjustment() const;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	///////////////////////////////////////////////////////////////////////////

	virtual TemperatureDependentSchedulerDTO toTemperatureDependentSchedulerDto() const override;
	virtual void updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual void createPersistedData() override;
	virtual void deletePersistedData() override;
	virtual void loadPersistedData() override;

	////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerImpl::PersistedData {
	const std::shared_ptr<TemperatureDependentSchedulerRepository> repository;

	const unsigned id;
	int remainingPercent;
	LocalDateTime lastRun;

public:
	PersistedData(
		const std::shared_ptr<TemperatureDependentSchedulerRepository>& repository,
		unsigned id
	);

	void init();
	void uninit();
	void save(int remainingPercent, const LocalDateTime& lastRun);
	void load(int& remainingPercent, LocalDateTime& lastRun);
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerImplFactory : public TemperatureDependentSchedulerFactory {
	const std::shared_ptr<TemperatureDependentSchedulerRepository> repository;
	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

public:
	TemperatureDependentSchedulerImplFactory(
			const std::shared_ptr<TemperatureDependentSchedulerRepository>& repository,
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);

	virtual ~TemperatureDependentSchedulerImplFactory() = default;
	virtual TemperatureDependentSchedulerPtr create(unsigned id) const override;
};
