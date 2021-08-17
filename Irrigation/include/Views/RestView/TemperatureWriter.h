#pragma once
#include <string>
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"


class TemperatureWriter {
public:
	virtual ~TemperatureWriter() = default;
	virtual std::string contentType() const = 0;
	virtual std::string currentToString(const std::string& datetimeStr, float value) const = 0;
	virtual std::string tomorrowToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) const = 0;
	virtual std::string yesterdayToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) const = 0;
	virtual std::string todayToString(const std::string& datetimeFromStr, const std::string& datetimeNowStr, const std::string& datetimeToStr, const TemperatureHistory::Values& historyValue, const TemperatureForecast::Values& forecastValue) const = 0;
};
