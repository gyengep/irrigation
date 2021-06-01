#pragma once
#include <string>
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"


class TemperatureWriter {
public:
	virtual ~TemperatureWriter() = default;
	virtual std::string contentType() const = 0;
	virtual std::string currentToString(float value) const = 0;
	virtual std::string forecastToString(const TemperatureForecast::Values& value, const DateTime& from, const DateTime& to) const = 0;
	virtual std::string historyToString(const TemperatureHistory::Values& value, const DateTime& from, const DateTime& to) const = 0;
};
