#pragma once
#include <chrono>
#include <memory>
#include <mutex>
#include "CurrentTemperature.h"
#include "TemperatureHistory.h"
#include "TemperatureHistoryPersister.h"
#include "Utils/DateTime.h"


class TemperatureHistoryImpl : public TemperatureHistory, public CurrentTemperatureListener {
	const std::shared_ptr<CurrentTemperature> currentTemperature;
	const std::shared_ptr<TemperatureHistoryPersister> temperatureHistoryPersister;
	const std::chrono::seconds historyLength;

	mutable std::mutex mtx;

public:
	TemperatureHistoryImpl(
			const std::shared_ptr<CurrentTemperature>& currentTemperature,
			const std::shared_ptr<TemperatureHistoryPersister>& temperatureHistoryPersister,
			const std::chrono::seconds& historyLength
		);

	virtual ~TemperatureHistoryImpl();

	void registerToListener();
	void unregisterFromListener();

	virtual Values getTemperatureHistory(const DateTime& from, const DateTime& to) const override;

	virtual void onTemperatureUpdated(const DateTime& dateTime, float temperature) override;
};
