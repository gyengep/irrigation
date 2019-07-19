#pragma once
#include <memory>
#include <string>
#include "TemperatureForecastProvider.h"
#include "NetworkReader.h"

namespace OpenWeatherMap {

class TemperatureForecastProvider : public ::TemperatureForecastProvider {
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

public:
	TemperatureForecastProvider();
	TemperatureForecastProvider(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureForecastProvider();

	virtual std::list<ValuesWithTimes> getForecast() const override;

	static std::list<ValuesWithTimes> parseXml(const std::string& text);
	static std::time_t parseTimeString(const std::string& text);
};

}
