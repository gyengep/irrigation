#include "RestViewTest.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"
#include <curl/curl.h>

using namespace std;


void RestViewTest::SetUp() {
	document = IrrigationDocument::Builder().build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));
}

void RestViewTest::TearDown() {
}

std::string RestViewTest::createUrl(const std::string& path) {
	if ('/' != path[0]) {
		throw std::logic_error("The path has to start with '/'");
	}

	std::ostringstream o;

	o << "http://localhost:" << port;
	o << path;
/*
	if (!parameters.empty()) {
		o << "?";

		for (auto it = parameters.begin(); it != parameters.end(); ++it) {
			if (parameters.begin() != it) {
				o << "&";
			}

			o << it->first << "=" << it->second;
		}
	}
*/
	return o.str();
}

RestViewTest::Response RestViewTest::executeRequest(const std::string& method, const std::string&  url, const std::string& sendData) {
	Response response;

	CURL *curl = curl_easy_init();
	if (curl == nullptr) {
		throw logic_error("RestViewTest::executeRequest()  curl == nullptr");
	}

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.writeCallbackData);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headerCallbackData);

	if (!sendData.empty()) {
		ReadCallbackData readCallbackData(sendData);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &readCallbackData);
	}

	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.responseCode);
	curl_easy_cleanup(curl);

    return response;
}

