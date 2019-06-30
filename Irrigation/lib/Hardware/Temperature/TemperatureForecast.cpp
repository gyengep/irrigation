#include "TemperatureForecast.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
#include "pugixml.hpp"
#include <curl/curl.h>
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace pugi;

const string TemperatureForecast::url("api.openweathermap.org/data/2.5/forecast");
const string TemperatureForecast::location("Dunakeszi,hu");
const string TemperatureForecast::appid("4560b35d4d7cfa41e7cdf944ddf59a58");

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
	minMaxValues(values)
{
}

bool TemperatureForecast::ValuesWithTimes::operator== (const ValuesWithTimes& other) const {
	return ((from == other.from) && (to == other.to) && (minMaxValues == other.minMaxValues));
}

ostream& operator<<(ostream& os, const TemperatureForecast::ValuesWithTimes& valuesWithTimes) {
	os << "ValuesWithTimes{";
	os << "from: " << valuesWithTimes.from << ", ";
	os << "to: " << valuesWithTimes.to << ", ";
	os << "values: " << valuesWithTimes.minMaxValues;
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

string TemperatureForecast::NetworkReader::read(const string& url, const string& location, const string& appid) const {
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

TemperatureForecast::TemperatureForecast(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

TemperatureForecast::~TemperatureForecast() {
}

void TemperatureForecast::updateCache() {
	list<ValuesWithTimes> temporaryTemperatures;

	try {
		temporaryTemperatures = parseXml(networkReader->read(url, location, appid));
	} catch (const exception& e) {
		LOGGER.warning("Can not query the weather forecast", e);
	}

	lock_guard<mutex> lock(mtx);
	temperatures.swap(temporaryTemperatures);
}

list<TemperatureForecast::ValuesWithTimes> TemperatureForecast::parseXml(const string& text) {
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

		addTemperatureTo(result, parseTimeString(from), parseTimeString(to), Values(min, max));

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

void TemperatureForecast::addTemperature(time_t from, time_t to, const Values& values) {
	lock_guard<mutex> lock(mtx);
	addTemperatureTo(temperatures, from, to, values);
}

void TemperatureForecast::addTemperatureTo(list<ValuesWithTimes>& temperatures, time_t from, time_t to, const Values& values) {
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

TemperatureForecast::Values TemperatureForecast::getForecastValues(time_t from, time_t to) const {
	lock_guard<mutex> lock(mtx);

	float min = numeric_limits<float>::max();
	float max = numeric_limits<float>::min();
	bool found = false;

	for (const auto& valuesWithTimes : temperatures) {
		if ((from < valuesWithTimes.to) && (valuesWithTimes.from < to)) {
			min = std::min(min, valuesWithTimes.minMaxValues.min);
			max = std::max(max, valuesWithTimes.minMaxValues.max);
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

void TemperatureForecast::startTimer(const std::chrono::duration<int64_t>& period) {
	timer.reset(new Timer(this, period, Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureForecast::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureForecast::onTimer() {
	LOGGER.trace("TemperatureForecast::onTimer()");
	updateCache();
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
