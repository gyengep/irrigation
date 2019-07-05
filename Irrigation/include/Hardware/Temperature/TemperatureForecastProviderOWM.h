#pragma once
#include <memory>
#include <string>
#include "TemperatureForecastProvider.h"


class TemperatureForecastProviderOWM : public TemperatureForecastProvider {
public:
	class  NetworkReader;

private:
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);

public:
	TemperatureForecastProviderOWM();
	TemperatureForecastProviderOWM(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureForecastProviderOWM();

	virtual std::list<ValuesWithTimes> getForecast() const override;

	static std::list<ValuesWithTimes> parseXml(const std::string& text);
	static std::time_t parseTimeString(const std::string& text);
};


class TemperatureForecastProviderOWM::NetworkReader {
public:
	virtual ~NetworkReader() = default;
	virtual std::string read(const std::string& url, const std::string& location, const std::string& appid) const;
};

