#pragma once
#include <chrono>
#include <memory>
#include "TemperatureException.h"
#include "TemperatureStatistics.h"
#include "Utils/Timer.h"

class TemperatureHistory;
class TemperatureSensor;
class TemperatureForecast;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;

	Timer timer;

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

public:

	virtual ~Temperature();

	float getTemperature();

	virtual void onTimer() override;

	static void init(
			const std::chrono::duration<int64_t>& sensorUpdatePeriod,
			const std::string& temperatureCacheFileName,
			const std::chrono::duration<int64_t>& temperatureCacheLength,
			const std::string& temperatureHistoryFileName,
			const std::chrono::duration<int64_t>& temperatureHistoryPeriod,
			const std::chrono::duration<int64_t>& forecastUpdatePeriod
		);
	static std::shared_ptr<Temperature> getInstancePtr();
};
