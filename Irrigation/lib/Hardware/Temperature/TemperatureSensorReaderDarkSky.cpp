#include "TemperatureSensorReaderDarkSky.h"
#include "Logger/Logger.h"
#include "json.hpp"
#include <sstream>

using namespace std;
using namespace nlohmann;

///////////////////////////////////////////////////////////////////////////////

const string DarkSky::TemperatureSensorReader::url("https://api.darksky.net/forecast");
const string DarkSky::TemperatureSensorReader::lat("47.497912");
const string DarkSky::TemperatureSensorReader::lon("19.040235");
const string DarkSky::TemperatureSensorReader::apikey("e20cfc71fdcaa9b194ece54203a16752");

///////////////////////////////////////////////////////////////////////////////

DarkSky::TemperatureSensorReader::TemperatureSensorReader() :
	TemperatureSensorReader(make_shared<NetworkReader>())
{
}

DarkSky::TemperatureSensorReader::TemperatureSensorReader(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

DarkSky::TemperatureSensorReader::~TemperatureSensorReader() {
}

float DarkSky::TemperatureSensorReader::read() {
	ostringstream oss;
	oss << url << "/" << apikey << "/" << lat << "," << lon << "?";
	oss << "exclude=minutely,hourly,daily,alerts,flags" << "&";
	oss << "units=si";

	try {
		return parseJson(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("DarkSky current temperature reading is failed"));
	}
}

float DarkSky::TemperatureSensorReader::parseJson(const string& text) {
	const auto json = json::parse(text);
	const auto currentlyIt = json.find("currently");
	if (json.end() == currentlyIt) {
		throw runtime_error("currently not found");
	}

	const auto temperatureIt = currentlyIt->find("temperature");
	if (currentlyIt->end() == temperatureIt) {
		throw runtime_error("temperature not found");
	}

	return temperatureIt->get<float>();
}
