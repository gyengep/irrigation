#include "TemperatureForecastProviderDarkSky.h"
#include "Logger/Logger.h"
#include "json.hpp"
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace nlohmann;

///////////////////////////////////////////////////////////////////////////////

const string DarkSky::TemperatureForecastProvider::url("https://api.darksky.net/forecast");
const string DarkSky::TemperatureForecastProvider::lat("47.497912");
const string DarkSky::TemperatureForecastProvider::lon("19.040235");
const string DarkSky::TemperatureForecastProvider::apikey("e20cfc71fdcaa9b194ece54203a16752");

///////////////////////////////////////////////////////////////////////////////

DarkSky::TemperatureForecastProvider::TemperatureForecastProvider() :
	TemperatureForecastProvider(make_shared<NetworkReader>())
{
}

DarkSky::TemperatureForecastProvider::TemperatureForecastProvider(const shared_ptr<NetworkReader>& networkReader) :
	networkReader(networkReader)
{
}

DarkSky::TemperatureForecastProvider::~TemperatureForecastProvider() {
}

list<TemperatureForecastProvider::ValuesWithTimes> DarkSky::TemperatureForecastProvider::getForecast() const {
	ostringstream oss;
	oss << url << "/" << apikey << "/" << lat << "," << lon << "?";
	oss << "exclude=currently,minutely,daily,alerts,flags" << "&";
	oss << "units=si";

	try {
		return parseJson(networkReader->read(oss.str()));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("DarkSky forecast reading is failed"));
	}
}

list<TemperatureForecastProvider::ValuesWithTimes> DarkSky::TemperatureForecastProvider::parseJson(const string& text) {
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

		const time_t time = timeIt->get<time_t>();
		const float temp = tempIt->get<float>();

		result.push_back(ValuesWithTimes(time, time + 3600, temp, temp));
	}

	return result;
}
