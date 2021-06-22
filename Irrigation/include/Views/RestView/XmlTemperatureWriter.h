#pragma once
#include <string>
#include "TemperatureWriter.h"


class XmlTemperatureWriter : public TemperatureWriter {
public:
	virtual std::string contentType() const override;
	virtual std::string currentToString(const std::string& datetimeStr, float value) const override;
	virtual std::string forecastToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) const override;
	virtual std::string historyToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) const override;

	static std::string toTemperatureValue(float value);
};
