#pragma once
#include <string>


class TemperatureYesterdayDTO {
	std::string datetimeFrom;
	std::string datetimeTo;
	std::string unit;
	float minValue;
	float maxValue;
	float avgValue;

public:
	TemperatureYesterdayDTO(const std::string& datetimeFrom, const std::string& datetimeTo, const std::string& unit, float minValue, float maxValue, float avgValue) :
		datetimeFrom(datetimeFrom),
		datetimeTo(datetimeTo),
		unit(unit),
		minValue(minValue),
		maxValue(maxValue),
		avgValue(avgValue)
	{
	}

	const std::string& getDateTimeFrom() const { return datetimeFrom; }
	const std::string& getDateTimeTo() const   { return datetimeTo; }
	const std::string& getUnit() const         { return unit; }
	float getMinValue() const                  { return minValue; }
	float getMaxValue() const                  { return maxValue; }
	float getAvgValue() const                  { return avgValue; }
};
