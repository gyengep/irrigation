#pragma once
#include <chrono>
#include <memory>
#include "DTO/HotWeatherSchedulerDTO.h"
#include "Persistence/HotWeatherSchedulerRepository.h"
#include "Temperature/TemperatureHistory.h"
#include "HotWeatherScheduler.h"


class HotWeatherSchedulerImpl : public HotWeatherScheduler {
public:
	class PersistedData;

private:
	const std::shared_ptr<PersistedData> persistedData;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

	DateTime lastRun;
	std::chrono::seconds period;
	float minTemperature;

public:
	HotWeatherSchedulerImpl(
			const std::shared_ptr<PersistedData>& persistedData,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);
	HotWeatherSchedulerImpl(
			const std::shared_ptr<PersistedData>& persistedData,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::chrono::seconds& period,
			float minTemperature
		);

	virtual ~HotWeatherSchedulerImpl();

	const DateTime& getLastRun() const { return lastRun; }
	float getMinTemperature() const;
	unsigned getPeriod() const;

	///////////////////////////////////////////////////////////////////////////

	virtual void setMinTemperature(float minTemperature) override;
	virtual void setPeriod(const std::chrono::seconds& period) override;

	virtual std::unique_ptr<Scheduler::Result> process(const LocalDateTime& localDateTime) override;

	////////////////////////////////////////////////////////////

	virtual HotWeatherSchedulerDTO toHotWeatherSchedulerDto() const override;
	virtual void updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO) override;

	////////////////////////////////////////////////////////////

	virtual void createPersistedData() override;
	virtual void deletePersistedData() override;
	virtual void loadPersistedData() override;

	////////////////////////////////////////////////////////////

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImpl::PersistedData {
	const std::shared_ptr<HotWeatherSchedulerRepository> repository;

	const unsigned id;
	LocalDateTime lastRun;

public:
	PersistedData(
		const std::shared_ptr<HotWeatherSchedulerRepository>& repository,
		unsigned id
	);

	void init();
	void uninit();
	void save(const DateTime& lastRun);
	void load(DateTime& lastRun);
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImplFactory : public HotWeatherSchedulerFactory {
	const std::shared_ptr<HotWeatherSchedulerRepository> repository;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;

public:
	HotWeatherSchedulerImplFactory(
			const std::shared_ptr<HotWeatherSchedulerRepository>& repository,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory
		);

	virtual ~HotWeatherSchedulerImplFactory() = default;
	virtual HotWeatherSchedulerPtr create(unsigned id) const override;
};
