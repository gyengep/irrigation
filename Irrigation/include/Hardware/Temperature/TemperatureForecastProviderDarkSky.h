#pragma once
#include <memory>
#include <string>
#include "TemperatureForecastProvider.h"
#include "NetworkReader.h"

namespace DarkSky {

class TemperatureForecastProvider : public ::TemperatureForecastProvider {
	static const std::string url;
	static const std::string lat, lon;
	static const std::string apikey;

	std::shared_ptr<NetworkReader> networkReader;

public:
	TemperatureForecastProvider();
	TemperatureForecastProvider(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureForecastProvider();

	virtual std::list<ValuesWithTimes> getForecast() const override;

	static std::list<ValuesWithTimes> parseJson(const std::string& text);
};

}
