#pragma once
#include <memory>
#include <mutex>
#include "TemperatureException.h"

class TemperatureSensor;
class TemperatureStatistics;


class Temperature {
	static std::shared_ptr<Temperature> instance;

	std::shared_ptr<TemperatureSensor> sensor;
	std::shared_ptr<TemperatureStatistics> statistics;

public:
	Temperature(const std::string& temperatureFileName);
	virtual ~Temperature();

	float getTemperature();

	static void init(const std::string& temperatureFileName);
	static std::shared_ptr<Temperature> getInstancePtr();
};
