#pragma once
#include <ctime>
#include <chrono>
#include <memory>
#include <mutex>
#include "CurrentTemperature.h"
#include "TemperatureHistory.h"
#include "TemperatureHistoryPersister.h"


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

	virtual Values getTemperatureHistory(const std::time_t& from, const std::time_t& to) const override;

	virtual void onTemperatureUpdated(const time_t& rawTime, float temperature) override;
};
