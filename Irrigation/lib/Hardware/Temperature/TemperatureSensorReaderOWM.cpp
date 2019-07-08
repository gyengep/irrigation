#include "TemperatureSensorReaderOWM.h"
#include "Logger/Logger.h"
#include "pugixml.hpp"
#include <curl/curl.h>
#include <sstream>

using namespace std;
using namespace pugi;


const string TemperatureSensorReader_OWM::url("api.openweathermap.org/data/2.5/weather");
const string TemperatureSensorReader_OWM::location("Dunakeszi,hu");
const string TemperatureSensorReader_OWM::appid("4560b35d4d7cfa41e7cdf944ddf59a58");

///////////////////////////////////////////////////////////////////////////////

string TemperatureSensorReader_OWM::NetworkReader::read(const string& url, const string& location, const string& appid) const {
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
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &TemperatureSensorReader_OWM::writeCallback);
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

TemperatureSensorReader_OWM::TemperatureSensorReader_OWM() : networkReader(make_shared<NetworkReader>()) {
}

TemperatureSensorReader_OWM::~TemperatureSensorReader_OWM() {
}

float TemperatureSensorReader_OWM::read() {
	const string text = networkReader->read(url, location, appid);
	//LOGGER.trace("TemperatureSensorReader_OWM::read(): %s", text.c_str());
	return parseXml(text);
}

float TemperatureSensorReader_OWM::parseXml(const string& text) {
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

size_t TemperatureSensorReader_OWM::writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	ostringstream* oss = static_cast<ostringstream*>(ctxt);

    if (!oss) {
    	throw logic_error("TemperatureForecastProviderOWM::writeCallback  nullptr == ctxt");
    }

    const size_t length = size * nmemb;
    (*oss) << string(&buffer[0], &buffer[length]);
   	return length;
}

