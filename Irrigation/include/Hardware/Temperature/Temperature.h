#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "TemperatureForecast.h"
#include "TemperatureStatistics.h"
#include "Utils/Timer.h"

class TemperatureHistory;
class TemperatureSensor;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;
	static const std::chrono::seconds::rep periodInSeconds;

	std::unique_ptr<Timer> timer;
	std::time_t lastUpdate;
	std::unique_ptr<TemperatureForecast::Values> forecastValues;
	std::time_t periodStart;
	std::time_t periodEnd;

	std::shared_ptr<TemperatureSensor> sensor;
	std::shared_ptr<TemperatureStatistics> statistics;
	std::shared_ptr<TemperatureHistory> history;
	std::shared_ptr<TemperatureForecast> forecast;

	Temperature(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureCacheFileName,
			const std::chrono::duration<int64_t>& temperatureCacheLength,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);

	std::shared_ptr<TemperatureSensor> createSensor();
	void logStoredForecast();
	void logPreviousPeriodMeasured(std::time_t currentTime);

	static std::string toTimeStr(time_t rawTime);

public:

	virtual ~Temperature();
	virtual void onTimer() override;

	static void init(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureCacheFileName,
			const std::chrono::duration<int64_t>& temperatureCacheLength,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);

	static void uninit();
	static std::shared_ptr<Temperature> getInstancePtr();
	static std::pair<std::time_t, std::time_t> getPreviousPeriod(std::time_t currentTime);
	static std::pair<std::time_t, std::time_t> getCurrentPeriod(std::time_t currentTime);
};
