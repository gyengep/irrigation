#pragma once
#include <chrono>
#include "TemperatureValues.h"


class TemperatureStatistics {
public:
	virtual ~TemperatureStatistics() = default;
	virtual void addTemperature(std::time_t rawTime, float temperature) = 0;
	virtual TemperatureValues getStatistics(std::time_t from, std::time_t to) = 0;
};
