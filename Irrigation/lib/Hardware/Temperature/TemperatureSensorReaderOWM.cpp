#include "TemperatureSensorReaderOWM.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "pugixml.hpp"
#include <sstream>

using namespace std;
using namespace pugi;

///////////////////////////////////////////////////////////////////////////////

const string OpenWeatherMap::TemperatureSensorReader::url("http://api.openweathermap.org/data/2.5/weather");
const string OpenWeatherMap::TemperatureSensorReader::location("dunakeszi,hu");
const string OpenWeatherMap::TemperatureSensorReader::appid("4560b35d4d7cfa41e7cdf944ddf59a58");

///////////////////////////////////////////////////////////////////////////////

OpenWeatherMap::TemperatureSensorReader::TemperatureSensorReader() :
	TemperatureSensorReader(make_shared<NetworkReader>())
{
}

OpenWeatherMap::TemperatureSensorReader::TemperatureSensorReader(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

OpenWeatherMap::TemperatureSensorReader::~TemperatureSensorReader() {
}

float OpenWeatherMap::TemperatureSensorReader::read() {
	ostringstream oss;
	oss << url << "?";
	oss << "q=" << location << "&";
	oss << "appid=" << appid << "&";
	oss << "mode=xml" << "&";
	oss << "units=metric";

	try {
		return parseXml(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("OpenWeatherMap current temperature reading is failed"));
	}
}

float OpenWeatherMap::TemperatureSensorReader::parseXml(const string& text) {
	static const pugi::xpath_query queryTemperature("/current/temperature");

	xml_document doc;
	const xml_parse_result parseResult = doc.load_string(text.c_str());

	if (status_ok != parseResult.status) {
		throw runtime_error(parseResult.description());
	}

	pugi::xpath_node_set temperatureNodeSet = queryTemperature.evaluate_node_set(doc);

	if (temperatureNodeSet.size() != 1) {
		throw runtime_error("Invalid XML, contains " + to_string(temperatureNodeSet.size()) + " temperature element");
	}

	return temperatureNodeSet.first().node().attribute("max").as_float();
}
