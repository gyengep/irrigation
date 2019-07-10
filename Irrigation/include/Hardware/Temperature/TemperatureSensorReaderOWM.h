#pragma once
#include <memory>
#include <string>
#include "TemperatureSensorReader.h"


class TemperatureSensorReader_OWM : public TemperatureSensorReader {
public:
	class  NetworkReader;

private:
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);

public:
	TemperatureSensorReader_OWM();
	virtual ~TemperatureSensorReader_OWM();

	virtual float read() override;

	static float parseXml(const std::string& text);
};


class TemperatureSensorReader_OWM::NetworkReader {
public:
	virtual ~NetworkReader() = default;
	virtual std::string read(const std::string& url, const std::string& location, const std::string& appid) const;
};

