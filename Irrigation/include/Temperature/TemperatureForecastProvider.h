#pragma once
#include <ctime>
#include <list>
#include <ostream>


class TemperatureForecastProvider {
public:

	struct ValuesWithTimes {
		const std::time_t from;
		const std::time_t to;
		const float min;
		const float max;

		ValuesWithTimes(const std::time_t from, const std::time_t to, float min, float max);

		// for testing
		bool operator== (const ValuesWithTimes& other) const;
		friend std::ostream& operator<<(std::ostream& os, const ValuesWithTimes& valuesWithTimes);
	};

	virtual ~TemperatureForecastProvider() = default;
	virtual std::list<ValuesWithTimes> readTemperatureForecast() const = 0;
	virtual std::string getForecastProviderName() const;
};
