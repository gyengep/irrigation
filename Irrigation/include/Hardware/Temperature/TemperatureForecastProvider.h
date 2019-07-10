#pragma once
#include <chrono>
#include <list>
#include <ostream>


class TemperatureForecastProvider {
public:

	struct ValuesWithTimes;

	virtual ~TemperatureForecastProvider() = default;
	virtual std::list<ValuesWithTimes> getForecast() const = 0;
};


struct TemperatureForecastProvider::ValuesWithTimes {
	const std::time_t from;
	const std::time_t to;
	const float min;
	const float max;

	ValuesWithTimes(const std::time_t from, const std::time_t to, float min, float max);

	// for testing
	bool operator== (const ValuesWithTimes& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ValuesWithTimes& valuesWithTimes);
};
