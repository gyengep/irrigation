#include "DarkSkyWrapper.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include "json.hpp"
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace nlohmann;

///////////////////////////////////////////////////////////////////////////////

const string DarkSkyWrapper::url("https://api.darksky.net/forecast");
const string DarkSkyWrapper::lat("47.497912");
const string DarkSkyWrapper::lon("19.040235");
const string DarkSkyWrapper::apikey("e20cfc71fdcaa9b194ece54203a16752");

///////////////////////////////////////////////////////////////////////////////

DarkSkyWrapper::DarkSkyWrapper() :
	DarkSkyWrapper(make_shared<NetworkReader>())
{
}

DarkSkyWrapper::DarkSkyWrapper(const std::shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

DarkSkyWrapper::~DarkSkyWrapper() {
}

std::string DarkSkyWrapper::getSensorName() const {
	return "DarkSky virtual";
}

std::string DarkSkyWrapper::getForecastProviderName() const {
	return "DarkSky";
}

float DarkSkyWrapper::readCurrentTemperature() const {
	ostringstream oss;
	oss << url << "/" << apikey << "/" << lat << "," << lon << "?";
	oss << "exclude=minutely,hourly,daily,alerts,flags" << "&";
	oss << "units=si";

	try {
		return parseCurrentTemperatureJson(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("DarkSky current temperature reading is failed"));
	}
}

list<TemperatureForecastProvider::ValuesWithTimes> DarkSkyWrapper::readTemperatureForecast() const {
	ostringstream oss;
	oss << url << "/" << apikey << "/" << lat << "," << lon << "?";
	oss << "exclude=currently,minutely,daily,alerts,flags" << "&";
	oss << "units=si";

	try {
		return parseTemperatureForecastJson(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("DarkSky forecast reading is failed"));
	}
}


float DarkSkyWrapper::parseCurrentTemperatureJson(const string& text) {
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

list<TemperatureForecastProvider::ValuesWithTimes> DarkSkyWrapper::parseTemperatureForecastJson(const string& text) {
	const auto json = json::parse(text);

	const auto hourlyIt = json.find("hourly");
	if (hourlyIt == json.end()) {
		throw runtime_error("hourly not found");
	}

	const auto dataArrayIt = hourlyIt->find("data");
	if (dataArrayIt == hourlyIt->end()) {
		throw runtime_error("data not found");
	}

	const auto& dataArray = *dataArrayIt;

	if (false == dataArray.is_array()) {
		throw runtime_error("data is not an array");
	}

	list<ValuesWithTimes> result;

	for (const auto& dataArrayItem : dataArray.items()) {
		const auto& data = dataArrayItem.value();
		const auto timeIt = data.find("time");
		const auto tempIt = data.find("temperature");

		if (timeIt == data.end()) {
			throw runtime_error("time not found");
		}

		if (tempIt == data.end()) {
			throw runtime_error("temperature not found");
		}

		const DateTime dateTime = DateTime::create(timeIt->get<time_t>());
		const float temp = tempIt->get<float>();

		result.push_back(ValuesWithTimes(dateTime, dateTime.addHours(1), temp, temp));
	}

	return result;
}
