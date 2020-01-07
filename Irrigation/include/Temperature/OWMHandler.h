#pragma once
#include <memory>
#include <string>
#include "CurrentTemperatureProvider.h"
#include "TemperatureForecastProvider.h"
#include "Utils/NetworkReader.h"


class OWMHandler : public TemperatureForecastProvider, public CurrentTemperatureProvider {
	static const std::string currentUrl;
	static const std::string forecastUrl;
	static const std::string location;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

public:
	OWMHandler();
	OWMHandler(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~OWMHandler();

	virtual float readCurrentTemperature() const override;
	virtual std::list<ValuesWithTimes> readTemperatureForecast() const override;

	virtual std::string getSensorName() const override;
	virtual std::string getForecastProviderName() const override;

	static float parseCurrentTemperatureXml(const std::string& text);
	static std::list<ValuesWithTimes> parseTemperatureForecastXml(const std::string& text);
	static std::time_t parseTimeString(const std::string& text);
};
