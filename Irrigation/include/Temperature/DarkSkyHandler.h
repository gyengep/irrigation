#pragma once
#include <memory>
#include <string>
#include "CurrentTemperatureProvider.h"
#include "TemperatureForecastProvider.h"
#include "Utils/NetworkReader.h"


class DarkSkyHandler : public TemperatureForecastProvider, public CurrentTemperatureProvider {
	static const std::string url;
	static const std::string lat, lon;
	static const std::string apikey;

	std::shared_ptr<NetworkReader> networkReader;

public:
	DarkSkyHandler();
	DarkSkyHandler(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~DarkSkyHandler();

	virtual float readCurrentTemperature() const override;
	virtual std::list<ValuesWithTimes> readTemperatureForecast() const override;

	virtual std::string getSensorName() const override;
	virtual std::string getForecastProviderName() const override;

	static float parseCurrentTemperatureJson(const std::string& text);
	static std::list<ValuesWithTimes> parseTemperatureForecastJson(const std::string& text);
};
