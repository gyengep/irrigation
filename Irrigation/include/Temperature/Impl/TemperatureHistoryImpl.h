#pragma once
#include <chrono>
#include <memory>
#include <mutex>
#include "Temperature/CurrentTemperature.h"
#include "Temperature/TemperatureHistory.h"
#include "Temperature/TemperatureHistoryRepository.h"
#include "Utils/DateTime.h"


class TemperatureHistoryImpl : public TemperatureHistory, public CurrentTemperatureListener {
	const std::shared_ptr<CurrentTemperature> currentTemperature;
	const std::shared_ptr<TemperatureHistoryRepository> temperatureHistoryRepository;
	const std::chrono::seconds historyLength;

	mutable std::mutex mtx;

public:
	TemperatureHistoryImpl(
			const std::shared_ptr<CurrentTemperature>& currentTemperature,
			const std::shared_ptr<TemperatureHistoryRepository>& temperatureHistoryRepository,
			const std::chrono::seconds& historyLength
		);

	virtual ~TemperatureHistoryImpl();

	void registerToListener();
	void unregisterFromListener();

	virtual Values getTemperatureHistory(const DateTime& from, const DateTime& to) const override;
	virtual void onTemperatureUpdated(const DateTime& dateTime, float temperature) override;
	virtual TemperatureHistoryDto toTemperatureHistoryDto(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat) const override;
};
