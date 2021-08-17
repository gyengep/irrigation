#pragma once
#include <string>
#include "TemperatureWriter.h"


class XmlTemperatureWriter : public TemperatureWriter {
public:
	virtual std::string contentType() const override;
	virtual std::string currentToString(const std::string& datetimeStr, float value) const override;
	virtual std::string tomorrowToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) const override;
	virtual std::string yesterdayToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) const override;
	virtual std::string todayToString(
			const std::string& datetimeFromStr, const std::string& datetimeNowStr, const std::string& datetimeToStr,
			const TemperatureHistory::Values& historyValue, const TemperatureForecast::Values& forecastValue
		) const override;

	static std::string toTemperatureValue(float value);
};
