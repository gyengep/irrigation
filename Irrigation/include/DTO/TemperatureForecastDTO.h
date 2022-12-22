#pragma once
#include <string>


class TemperatureForecastDTO {
	std::string datetimeFrom;
	std::string datetimeTo;
	std::string unit;
	float minValue;
	float maxValue;

public:
	TemperatureForecastDTO(const std::string& datetimeFrom, const std::string& datetimeTo, const std::string& unit, float minValue, float maxValue) :
		datetimeFrom(datetimeFrom),
		datetimeTo(datetimeTo),
		unit(unit),
		minValue(minValue),
		maxValue(maxValue)
	{
	}

	const std::string& getDateTimeFrom() const { return datetimeFrom; }
	const std::string& getDateTimeTo() const   { return datetimeTo; }
	const std::string& getUnit() const         { return unit; }
	float getMinValue() const                  { return minValue; }
	float getMaxValue() const                  { return maxValue; }
};
