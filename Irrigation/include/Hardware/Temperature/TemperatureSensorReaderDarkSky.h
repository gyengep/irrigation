#pragma once
#include <memory>
#include <string>
#include "TemperatureSensorReader.h"
#include "NetworkReader.h"

namespace DarkSky {

class TemperatureSensorReader : public ::TemperatureSensorReader {
	static const std::string url;
	static const std::string lat, lon;
	static const std::string apikey;

	std::shared_ptr<NetworkReader> networkReader;

public:
	TemperatureSensorReader();
	TemperatureSensorReader(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureSensorReader();

	virtual float read() override;

	static float parseJson(const std::string& text);
};

}
