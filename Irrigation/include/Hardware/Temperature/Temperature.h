#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "TemperatureForecast.h"
#include "Utils/Timer.h"

class TemperatureSensorImpl;
class TemperatureSensorReader;
class TemperatureHistoryImpl;
class TemperatureHistoryPersister;
class TemperatureForecast;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;
	static const std::chrono::seconds::rep periodInSeconds;

	std::chrono::system_clock::time_point lastUpdateTime;
	std::chrono::duration<int64_t> period;
	std::unique_ptr<Timer> timer;

	std::shared_ptr<TemperatureSensorImpl> sensor;
	std::shared_ptr<TemperatureHistoryImpl> history;
	std::shared_ptr<TemperatureHistoryPersister> historyPersister;
	std::shared_ptr<TemperatureForecast> forecast;

	std::string storedForecastFrom, storedForecastTo;
	std::unique_ptr<TemperatureForecast::Values> storedForecastValues;

	Temperature(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryLength,
			const std::string& temperatureHistoryPersisterFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);

	std::shared_ptr<TemperatureSensorReader> createSensorReader();

	static void logCurrentPeriodForecast(
			const std::chrono::system_clock::time_point& from,
			const std::chrono::system_clock::time_point& to,
			const std::unique_ptr<TemperatureForecast::Values>& forecastValues
		);

	void logPreviousPeriodMeasured(const std::chrono::system_clock::time_point& timePoint);
	void logStoredPeriodForecast();
	void logCurrentPeriodForecast(const std::chrono::system_clock::time_point& timePoint);

	static std::string toTimeStr(const std::chrono::system_clock::time_point& timePoint);

public:

	virtual ~Temperature();
	virtual void onTimer() override;

	static void init(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryLength,
			const std::string& temperatureHistoryPersisterFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);

	static void uninit();
	static std::shared_ptr<Temperature> getInstancePtr();
};
