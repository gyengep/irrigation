#pragma once
#include <ctime>
#include <ostream>


class TemperatureForecast {
public:
	struct Values;

	virtual ~TemperatureForecast() = default;
	virtual Values getForecastValues(const std::time_t& from, const std::time_t& to) const = 0;
};


struct TemperatureForecast::Values {
	const float min;
	const float max;

	Values(float min, float max);

	// for testing
	bool operator== (const Values& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Values& values);
};
