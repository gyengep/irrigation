#pragma once
#include <vector>


class TemperatureToPercent {
	std::vector<std::pair<float, int>> temperatureAndPercents;

	TemperatureToPercent();

public:
	virtual ~TemperatureToPercent();

	void setTemperatureAndPercents(const std::vector<std::pair<float, int>>& temperatureAndPercents);
	int getRequiredPercentFromTemperature(float temperature) const;

	static TemperatureToPercent& getInstance();
};
