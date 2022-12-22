#pragma once
#include <string>


class CurrentTemperatureDTO {
	std::string datetime;
	std::string unit;
	float value;

public:
	CurrentTemperatureDTO(const std::string& datetime, const std::string& unit, float value) :
		datetime(datetime),
		unit(unit),
		value(value)
	{
	}

	const std::string& getDateTime() const { return datetime; }
	const std::string& getUnit() const     { return unit; }
	float getValue() const                 { return value; }
};
