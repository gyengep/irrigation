#include "NetworkReader.h"
#include <curl/curl.h>
#include <cstring>
#include <memory>
#include <sstream>

using namespace std;


NetworkReader::NetworkReader() {
}

NetworkReader::~NetworkReader() {
}

string NetworkReader::read(const string& url) const {
	unique_ptr<CURL, void(*)(CURL*)> curl(curl_easy_init(), curl_easy_cleanup);

	if (curl.get() == nullptr) {
		throw logic_error("TemperatureForecast::read()  curl == nullptr");
	}

	ostringstream response;
	long responseCode = 0;
	char errbuf[CURL_ERROR_SIZE] = {0};

	curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &writeCallback);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 0L);

	const CURLcode curlCode = curl_easy_perform(curl.get());
	if (CURLE_OK != curlCode) {
		string errorMessage = (strlen(errbuf) > 0) ? errbuf : curl_easy_strerror(curlCode);
		throw runtime_error("curl_easy_perform() failed: " + to_string(curlCode) + ", " + errorMessage);
	}

	curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &responseCode);
	if (responseCode < 200 || 300 <= responseCode) {
		throw runtime_error("http status code: " + to_string(responseCode) + ", message: " + response.str());
	}

	return move(response.str());
}

size_t NetworkReader::writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	ostringstream* oss = static_cast<ostringstream*>(ctxt);

	if (!oss) {
		throw logic_error("NetworkReader::writeCallback() nullptr == ctxt");
	}

	const size_t length = size * nmemb;
	(*oss) << string(&buffer[0], &buffer[length]);
	return length;
}

