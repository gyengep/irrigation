#pragma once
#include <list>
#include <ostream>
#include "Utils/DateTime.h"


class TemperatureForecastProvider {
public:

	struct ValuesWithTimes {
		const DateTime from;
		const DateTime to;
		const float min;
		const float max;

		ValuesWithTimes(const DateTime& from, const DateTime& to, float min, float max);

		// for testing
		bool operator== (const ValuesWithTimes& other) const;
		friend std::ostream& operator<<(std::ostream& os, const ValuesWithTimes& valuesWithTimes);
	};

	virtual ~TemperatureForecastProvider() = default;
	virtual std::list<ValuesWithTimes> readTemperatureForecast() const = 0;
	virtual std::string getForecastProviderName() const = 0;
};
