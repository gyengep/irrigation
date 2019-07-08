#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "Utils/Timer.h"

class TemperatureSensorImpl;
class TemperatureSensorReader;
class TemperatureHistoryImpl;
class TemperatureHistoryPersister;
class TemperatureForecast;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;
	static const std::chrono::seconds::rep periodInSeconds;

	std::unique_ptr<Timer> timer;

	std::shared_ptr<TemperatureSensorImpl> sensor;
	std::shared_ptr<TemperatureHistoryImpl> history;
	std::shared_ptr<TemperatureHistoryPersister> historyPersister;
	std::shared_ptr<TemperatureForecast> forecast;

	Temperature(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryLength,
			const std::string& temperatureHistoryPersisterFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);

	std::shared_ptr<TemperatureSensorReader> createSensorReader();

	void logStoredForecast();
	void logPreviousPeriodMeasured(std::time_t currentTime);

	static std::string toTimeStr(time_t rawTime);

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
