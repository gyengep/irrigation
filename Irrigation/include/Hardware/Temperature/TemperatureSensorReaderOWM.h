#pragma once
#include <memory>
#include <string>
#include "TemperatureSensorReader.h"
#include "NetworkReader.h"

namespace OpenWeatherMap {

class TemperatureSensorReader : public ::TemperatureSensorReader {
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	std::shared_ptr<NetworkReader> networkReader;

public:
	TemperatureSensorReader();
	TemperatureSensorReader(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureSensorReader();

	virtual float read() override;

	static float parseXml(const std::string& text);
};

}
