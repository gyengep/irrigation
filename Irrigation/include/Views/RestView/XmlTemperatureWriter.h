#pragma once
#include <string>
#include "TemperatureWriter.h"


class XmlTemperatureWriter : public TemperatureWriter {
public:
	virtual std::string contentType() const override;
	virtual std::string currentToString(float value) const override;
	virtual std::string forecastToString(const TemperatureForecast::Values& value, const DateTime& from, const DateTime& to) const override;
	virtual std::string historyToString(const TemperatureHistory::Values& value, const DateTime& from, const DateTime& to) const override;

	static std::string toTemperatureValue(float value);
};
