#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "TemperatureHistory.h"
#include "TemperatureForecast.h"
#include "Utils/Timer.h"

class TemperatureSensorImpl;
class TemperatureSensorReader;
class TemperatureHistoryImpl;
class TemperatureHistoryPersister;
class TemperatureForecastImpl;
class TemperatureForecastProvider;


class Temperature : public TimerCallback {
	static const std::chrono::seconds::rep periodInSeconds;

	std::time_t lastUpdateTime;
	std::chrono::duration<int64_t> period;
	std::unique_ptr<Timer> timer;

	std::shared_ptr<TemperatureSensorImpl> sensor;
	std::shared_ptr<TemperatureHistoryImpl> history;
	std::shared_ptr<TemperatureHistoryPersister> historyPersister;
	std::shared_ptr<TemperatureForecastImpl> forecast;

	std::string storedForecastFrom, storedForecastTo;
	std::unique_ptr<TemperatureForecast::Values> storedForecastValues;

	Temperature();

	std::shared_ptr<TemperatureSensorReader> createSensorReader();
	std::shared_ptr<TemperatureForecastProvider> createForecastProvider();

	void logPreviousPeriodMeasured(const std::time_t& rawTime);
	void logStoredPeriodForecast();
	void logCurrentPeriodForecast(const std::time_t& rawTime);

public:

	virtual ~Temperature();
	virtual void onTimer() override;

	const std::shared_ptr<TemperatureHistory> getTemperatureHistory() const;
	const std::shared_ptr<TemperatureForecast> getTemperatureForecast() const;

	void init(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryLength,
			const std::string& temperatureHistoryPersisterFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);
	void uninit();

	static Temperature& getInstance();
};
