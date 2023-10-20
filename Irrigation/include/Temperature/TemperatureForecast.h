#pragma once
#include <ostream>
#include <string>
#include "DTO/TemperatureForecastDto.h"
#include "Utils/DateTime.h"


class TemperatureForecast {
public:

	struct Values {
		const float min;
		const float max;

		Values(float min, float max);

		// for testing
		bool operator== (const Values& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Values& values);
	};

	virtual ~TemperatureForecast() = default;
	virtual Values getTemperatureForecast(const DateTime& from, const DateTime& to) const = 0;
	virtual TemperatureForecastDto toTemperatureForecastDto(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat) const = 0;
};


