#pragma once
#include <memory>
#include <mutex>
#include "TemperatureException.h"
#include "TemperatureStatistics.h"
#include "Utils/Timer.h"

class TemperaturePersister;
class TemperatureSensor;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;

	Timer timer;

	std::shared_ptr<TemperatureSensor> sensor;
	std::shared_ptr<TemperatureStatistics> statistics;
	std::shared_ptr<TemperaturePersister> persister;

public:
	Temperature(const std::string& temperatureCacheFileName, const std::string& temperatureHistoryFileName);
	virtual ~Temperature();

	float getTemperature();
	StatisticsValues getStatistics(const std::chrono::duration<int64_t>& period);

	virtual void onTimer() override;

	static void init(const std::string& temperatureCacheFileName, const std::string& temperatureHistoryFileName);
	static std::shared_ptr<Temperature> getInstancePtr();
};
