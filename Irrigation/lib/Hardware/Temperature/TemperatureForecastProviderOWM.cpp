#include "TemperatureForecastProviderOWM.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
#include "pugixml.hpp"
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace pugi;

///////////////////////////////////////////////////////////////////////////////

const string OpenWeatherMap::TemperatureForecastProvider::url("http://api.openweathermap.org/data/2.5/forecast");
const string OpenWeatherMap::TemperatureForecastProvider::location("dunakeszi,hu");
const string OpenWeatherMap::TemperatureForecastProvider::appid("4560b35d4d7cfa41e7cdf944ddf59a58");

///////////////////////////////////////////////////////////////////////////////

OpenWeatherMap::TemperatureForecastProvider::TemperatureForecastProvider() :
	TemperatureForecastProvider(make_shared<NetworkReader>())
{
}

OpenWeatherMap::TemperatureForecastProvider::TemperatureForecastProvider(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

OpenWeatherMap::TemperatureForecastProvider::~TemperatureForecastProvider() {
}

list<TemperatureForecastProvider::ValuesWithTimes> OpenWeatherMap::TemperatureForecastProvider::getForecast() const {
	ostringstream oss;
	oss << url << "?";
	oss << "q=" << location << "&";
	oss << "appid=" << appid << "&";
	oss << "mode=xml" << "&";
	oss << "units=metric";

	try {
		return parseXml(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("OpenWeatherMap forecast reading is failed"));
	}
}

list<TemperatureForecastProvider::ValuesWithTimes> OpenWeatherMap::TemperatureForecastProvider::parseXml(const string& text) {
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
		const string from = temperatureNode.parent().attribute("from").value();
		const string to = temperatureNode.parent().attribute("to").value();
		const float value = temperatureNode.node().attribute("value").as_float();
		const float min = temperatureNode.node().attribute("min").as_float();
		const float max = temperatureNode.node().attribute("max").as_float();

		result.push_back(TemperatureForecastProvider::ValuesWithTimes(
				parseTimeString(from),
				parseTimeString(to),
				min,
				max
			));

		if (false && LOGGER.isLoggable(LogLevel::TRACE)) {
			LOGGER.trace("unit:  %s", unit.c_str());
			LOGGER.trace("value: %f", value);
			LOGGER.trace("from:  %s %lld", from.c_str(), (int64_t)parseTimeString(from));
			LOGGER.trace("to:    %s %lld", to.c_str(), (int64_t)parseTimeString(to));
			LOGGER.trace("*********************");
		}
	}

	return result;
}

time_t OpenWeatherMap::TemperatureForecastProvider::parseTimeString(const string& text) {
	const static regex dateRegex("(\\d{4})-(\\d{2})-(\\d{2})T(\\d{2}):(\\d{2}):(\\d{2})");
	smatch dateMatch;

	if (false == regex_match(text, dateMatch, dateRegex)) {
		throw runtime_error("Invalid Time format");
	}

	if (dateMatch.size() != 7) {
		throw logic_error("TemperatureForecast::parseTimeString() dateMatch.size() != 7");
	}

	auto calendarTime = toCalendarTime(
			stoi(dateMatch[1]), stoi(dateMatch[2]), stoi(dateMatch[3]),
			stoi(dateMatch[4]), stoi(dateMatch[5]), stoi(dateMatch[6])
		);

	return timegm(&calendarTime);
}
