#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "TemperatureHistory.h"
#include "TemperatureForecast.h"

class CurrentTemperatureProvider;
class CurrentTemperatureImpl;
class TemperatureHistoryImpl;
class TemperatureHistoryPersister;
class TemperatureForecastImpl;
class TemperatureForecastProvider;


class Temperature {

	std::shared_ptr<CurrentTemperatureImpl> current;
	std::shared_ptr<TemperatureForecastImpl> forecast;
	std::shared_ptr<TemperatureHistoryImpl> history;
	std::shared_ptr<TemperatureHistoryPersister> historyPersister;


	Temperature();

	std::shared_ptr<CurrentTemperatureProvider> createCurrentTemperatureProvider();

public:

	virtual ~Temperature();

	const std::shared_ptr<TemperatureHistory> getTemperatureHistory() const;
	const std::shared_ptr<TemperatureForecast> getTemperatureForecast() const;

	void init(
			const std::chrono::duration<int64_t>& currentTemperatureUpdatePeriod,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryLength,
			const std::string& temperatureHistoryPersisterFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);
	void uninit();

	static Temperature& getInstance();
};
