#include "TemperatureForecast.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
#include "pugixml.hpp"
#include <curl/curl.h>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace pugi;

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::Values::Values(float min, float max) :
	min(min),
	max(max)
{
}

bool TemperatureForecast::Values::operator== (const Values& other) const {
	return ((min == other.min) && (max == other.max));
}

ostream& operator<<(ostream& os, const TemperatureForecast::Values& values) {
	os << "Values{";
	os << "min: " << values.min << ", ";
	os << "max: " << values.max;
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::ValuesWithTimes::ValuesWithTimes(time_t from, time_t to, const Values& values) :
	from(from),
	to(to),
	values(values)
{
}

bool TemperatureForecast::ValuesWithTimes::operator== (const ValuesWithTimes& other) const {
	return ((from == other.from) && (to == other.to) && (values == other.values));
}

ostream& operator<<(ostream& os, const TemperatureForecast::ValuesWithTimes& valuesWithTimes) {
	os << "ValuesWithTimes{";
	os << "from: " << valuesWithTimes.from << ", ";
	os << "to: " << valuesWithTimes.to << ", ";
	os << "values: " << valuesWithTimes.values;
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

string TemperatureForecast::NetworkReader::read(const string url, const string location, const string appid) const {
	unique_ptr<CURL, void(*)(CURL*)> curl(curl_easy_init(), curl_easy_cleanup);

	if (curl.get() == nullptr) {
		throw logic_error("TemperatureForecast::read()  curl == nullptr");
	}

	ostringstream oss;
	oss << url << "?";
	oss << "q=" << location << "&";
	oss << "appid=" << appid << "&";
	oss << "mode=xml" << "&";
	oss << "units=metric";

	ostringstream response;
	long responseCode = 0;

	curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_URL, oss.str().c_str());
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &TemperatureForecast::writeCallback);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

	const CURLcode curlCode = curl_easy_perform(curl.get());
	if (CURLE_OK != curlCode) {
		throw runtime_error("curl_easy_perform() failed. " + to_string(curlCode));
	}

	curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &responseCode);
	if (responseCode < 200 || 300 <= responseCode) {
		throw runtime_error("http status code: " + to_string(responseCode));
	}

	return move(response.str());
}

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::TemperatureForecast() :
	TemperatureForecast(make_shared<NetworkReader>())
{
}

TemperatureForecast::TemperatureForecast(const shared_ptr<NetworkReader>& networkReader) :
	url("api.openweathermap.org/data/2.5/forecast"),
	location("Dunakeszi,hu"),
	appid("4560b35d4d7cfa41e7cdf944ddf59a58"),
	networkReader(networkReader),
	timer(*this, chrono::minutes(60))
{
}

TemperatureForecast::~TemperatureForecast() {
}

void TemperatureForecast::updateCache() {
	std::list<ValuesWithTimes> temporaryTemperatures;

	try {
		const string responseXml = networkReader->read(url, location, appid);

		xml_document doc;
		xml_parse_result result = doc.load_string(responseXml.c_str());

		if (status_ok != result.status) {
			throw runtime_error(result.description());
		}

		static const pugi::xpath_query queryTemperature("/weatherdata/forecast/time/temperature");
		pugi::xpath_node_set temperatureNodeSet = queryTemperature.evaluate_node_set(doc);

		for (const auto& temperatureNode : temperatureNodeSet) {
			const string unit = temperatureNode.node().attribute("unit").value();
			const string from = temperatureNode.parent().attribute("from").value();
			const string to = temperatureNode.parent().attribute("to").value();
			const float value = temperatureNode.node().attribute("value").as_float();
			const float min = temperatureNode.node().attribute("min").as_float();
			const float max = temperatureNode.node().attribute("max").as_float();

			addTemperature(temporaryTemperatures, parseTimeString(from), parseTimeString(to), Values(min, max));

			if (false && LOGGER.isLoggable(LogLevel::TRACE)) {
				LOGGER.trace("unit:  %s", unit.c_str());
				LOGGER.trace("value: %f", value);
				LOGGER.trace("from:  %s %lld", from.c_str(), (int64_t)parseTimeString(from));
				LOGGER.trace("to:    %s %lld", to.c_str(), (int64_t)parseTimeString(to));
				LOGGER.trace("*********************");
			}
		}

	} catch (const exception& e) {
		LOGGER.warning("Can not query the weather forecast", e);
		temporaryTemperatures.clear();
	}

	lock_guard<mutex> lock(mtx);
	temperatures.swap(temporaryTemperatures);
}

void TemperatureForecast::addTemperature(time_t from, time_t to, const Values& values) {
	lock_guard<mutex> lock(mtx);
	addTemperature(temperatures, from, to, values);
}

TemperatureForecast::Values TemperatureForecast::getForecast(time_t from, time_t to) const {
	lock_guard<mutex> lock(mtx);

	float min = numeric_limits<float>::max();
	float max = numeric_limits<float>::min();
	bool found = false;

	for (const auto& valuesWithTimes : temperatures) {
		if ((from < valuesWithTimes.to) && (valuesWithTimes.from < to)) {
			min = std::min(min, valuesWithTimes.values.min);
			max = std::max(max, valuesWithTimes.values.max);
			found = true;
		}
	}

	if (false == found) {
		throw NoSuchElementException("Temperature forecast not available with specified criteria");
	}

	return Values(min, max);
}

const list<TemperatureForecast::ValuesWithTimes> TemperatureForecast::getContainer() const {
	lock_guard<mutex> lock(mtx);
	return temperatures;
}

void TemperatureForecast::startTimer() {
	timer.start();
}

void TemperatureForecast::stopTimer() {
	timer.stop();
}

void TemperatureForecast::onTimer() {
	updateCache();

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		const auto currentTime = time(nullptr);
		const auto values = getForecast(currentTime, currentTime + 60 * 60 *24);
		LOGGER.trace("Temperature forecast for the next 24 hours: min: %f max: %f", values.min, values.max);
	}
}

size_t TemperatureForecast::writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	ostringstream* oss = static_cast<ostringstream*>(ctxt);

    if (!oss) {
    	throw logic_error("TemperatureForecast::writeCallback  nullptr == ctxt");
    }

    const size_t length = size * nmemb;
    (*oss) << string(&buffer[0], &buffer[length]);
   	return length;
}

time_t TemperatureForecast::parseTimeString(const string& text) {
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

void TemperatureForecast::addTemperature(list<ValuesWithTimes>& temperatures, std::time_t from, std::time_t to, const Values& values) {
	if (from >= to) {
		throw runtime_error("Temperature forecast period from/to mismatch");
	}

	if (!temperatures.empty()) {
		if (temperatures.back().to != from) {
			throw runtime_error("Temperature forecast period from/to mismatch");
		}
	}

	temperatures.emplace_back(from, to, values);
}
