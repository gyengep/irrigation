#pragma once
#include <memory>
#include <string>
#include "CurrentTemperatureProvider.h"
#include "TemperatureForecastProvider.h"
#include "Utils/NetworkReader.h"


class OWMWrapper : public TemperatureForecastProvider, public CurrentTemperatureProvider {
	static const std::string currentUrl;
	static const std::string forecastUrl;
	static const std::string lat, lon;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

public:
	OWMWrapper();
	OWMWrapper(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~OWMWrapper();

	virtual float readCurrentTemperature() const override;
	virtual std::list<ValuesWithTimes> readTemperatureForecast() const override;

	virtual std::string getSensorName() const override;
	virtual std::string getForecastProviderName() const override;

	static float parseCurrentTemperatureXml(const std::string& text);
	static std::list<ValuesWithTimes> parseTemperatureForecastXml(const std::string& text);
	static UtcDateTime parseTimeString(const std::string& text);
};
