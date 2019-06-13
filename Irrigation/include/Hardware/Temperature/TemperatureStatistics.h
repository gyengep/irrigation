#pragma once
#include <chrono>


struct StatisticsValues {
	float minTemperature;
	float maxTemperature;
	float avgTemperature;
};

class TemperatureStatistics {
public:
	virtual ~TemperatureStatistics() = default;
	virtual void addTemperature(std::time_t rawTime, float temperature) = 0;
	virtual StatisticsValues getStatistics(std::time_t from, std::time_t to) = 0;
};
