#pragma once
#include <vector>


class TemperatureToPercent {
	std::vector<std::pair<float, unsigned>> temperatureAndPercents;

	TemperatureToPercent();

public:
	virtual ~TemperatureToPercent();

	void setTemperatureAndPercents(const std::vector<std::pair<float, unsigned>>& temperatureAndPercents);
	unsigned getRequiredPercentFromTemperature(float temperature) const;

	static TemperatureToPercent& getInstance();
};
