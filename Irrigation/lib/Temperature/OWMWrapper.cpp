#include "OWMWrapper.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include <pugixml/pugixml.hpp>
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace pugi;

///////////////////////////////////////////////////////////////////////////////

const string OWMWrapper::currentUrl("http://api.openweathermap.org/data/2.5/weather");
const string OWMWrapper::forecastUrl("http://api.openweathermap.org/data/2.5/forecast");
const string OWMWrapper::lat("47.63641");
const string OWMWrapper::lon("19.13864");
const string OWMWrapper::appid("4560b35d4d7cfa41e7cdf944ddf59a58");

///////////////////////////////////////////////////////////////////////////////

OWMWrapper::OWMWrapper() :
		OWMWrapper(make_shared<NetworkReader>())
{
}

OWMWrapper::OWMWrapper(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

OWMWrapper::~OWMWrapper() {
}

std::string OWMWrapper::getSensorName() const {
	return "OpenWeatherMap virtual";
}

std::string OWMWrapper::getForecastProviderName() const {
	return "OpenWeatherMap";
}

float OWMWrapper::readCurrentTemperature() const {
	ostringstream oss;
	oss << currentUrl << "?";
	oss << "lat=" << lat << "&";
	oss << "lon=" << lon << "&";
	oss << "appid=" << appid << "&";
	oss << "mode=xml" << "&";
	oss << "units=metric";

	try {
		return parseCurrentTemperatureXml(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("OpenWeatherMap current temperature reading is failed"));
	}
}

list<TemperatureForecastProvider::ValuesWithTimes> OWMWrapper::readTemperatureForecast() const {
	ostringstream oss;
	oss << forecastUrl << "?";
	oss << "lat=" << lat << "&";
	oss << "lon=" << lon << "&";
	oss << "appid=" << appid << "&";
	oss << "mode=xml" << "&";
	oss << "units=metric";

	try {
		return parseTemperatureForecastXml(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("OpenWeatherMap forecast reading is failed"));
	}
}

float OWMWrapper::parseCurrentTemperatureXml(const string& text) {
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

list<TemperatureForecastProvider::ValuesWithTimes> OWMWrapper::parseTemperatureForecastXml(const string& text) {
	static const pugi::xpath_query queryTemperature("/weatherdata/forecast/time/temperature");

	xml_document doc;
	const xml_parse_result parseResult = doc.load_string(text.c_str());

	if (status_ok != parseResult.status) {
		throw runtime_error(parseResult.description());
	}

	pugi::xpath_node_set temperatureNodeSet = queryTemperature.evaluate_node_set(doc);
	list<ValuesWithTimes> result;

	for (const auto& temperatureNode : temperatureNodeSet) {
		const string unit = temperatureNode.node().attribute("unit").value();
		const string fromText = temperatureNode.parent().attribute("from").value();
		const string toText = temperatureNode.parent().attribute("to").value();
		const float value = temperatureNode.node().attribute("value").as_float();
		const float min = temperatureNode.node().attribute("min").as_float();
		const float max = temperatureNode.node().attribute("max").as_float();

		const UtcDateTime from = parseTimeString(fromText);
		const UtcDateTime to = parseTimeString(toText);

		result.push_back(TemperatureForecastProvider::ValuesWithTimes(from, to, min, max));

		if (false && LOGGER.isLoggable(LogLevel::TRACE)) {
			LOGGER.trace("unit:  %s", unit.c_str());
			LOGGER.trace("value: %f", value);
			LOGGER.trace("from:  %s %s", fromText.c_str(), from.toString().c_str());
			LOGGER.trace("to:    %s %s", toText.c_str(), to.toString().c_str());
			LOGGER.trace("*********************");
		}
	}

	return result;
}

UtcDateTime OWMWrapper::parseTimeString(const string& text) {
	const static regex dateRegex("(\\d{4})-(\\d{2})-(\\d{2})T(\\d{2}):(\\d{2}):(\\d{2})");
	smatch dateMatch;

	if (false == regex_match(text, dateMatch, dateRegex)) {
		throw runtime_error("Invalid Time format");
	}

	if (dateMatch.size() != 7) {
		throw logic_error("OWMWrapper::parseTimeString() dateMatch.size() != 7");
	}

	return UtcDateTime(
			stoi(dateMatch[1]), stoi(dateMatch[2]), stoi(dateMatch[3]),
			stoi(dateMatch[4]), stoi(dateMatch[5]), stoi(dateMatch[6])
		);
}
