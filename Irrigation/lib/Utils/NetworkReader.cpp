#include "NetworkReader.h"
#include "CurlStringWriter.h"
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
		throw logic_error("NetworkReader::read()  curl == nullptr");
	}

	CurlStringWriter curlStringWriter;
	char errbuf[CURL_ERROR_SIZE] = {0};

	curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, CurlStringWriter::writeFunction);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &curlStringWriter);
	curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 0L);

	const CURLcode curlCode = curl_easy_perform(curl.get());
	if (CURLE_OK != curlCode) {
		const char* errorMessage = errbuf;

		if (errorMessage[0] == '\0') {
			errorMessage = curl_easy_strerror(curlCode);
		}

		throw runtime_error("curl_easy_perform() failed: " + to_string(curlCode) + ", " + errorMessage);
	}

	long responseCode = 0;
	curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &responseCode);
	if (responseCode < 200 || 300 <= responseCode) {
		throw runtime_error("http status code: " + to_string(responseCode) + ", message: " + curlStringWriter.getText());
	}

	return curlStringWriter.getText();
}
