#include "RestViewTest.h"
#include "Model/IrrigationDocument.h"
#include "Utils/DateTime.h"
#include "Utils/CurlStringReader.h"
#include "Views/RestView/RestView.h"
#include <curl/curl.h>

using namespace std;
using namespace testing;


void RestViewTest::SetUp() {
	mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	mockTemperatureHistory = std::make_shared<MockTemperatureHistory>();
	mockTemperatureForecast = std::make_shared<MockTemperatureForecast>();
	mockShutdownManager = std::make_shared<MockShutdownManager>();

	irrigationDocument = IrrigationDocument::Builder().build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager
		)));
}

void RestViewTest::TearDown() {
	DateTime::resetTimefunc();
}

string RestViewTest::createUrl(const string& path) {
	if ('/' != path[0]) {
		throw logic_error("The path has to start with '/'");
	}

	ostringstream o;

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

RestViewTest::Response RestViewTest::__executeRequest__(const string& method, const string&  url, const string& body, const string& headerField) {
	Response response;

	CURL *curl = curl_easy_init();
	curl_slist *header = NULL;
	CurlStringReader curlStringReader(body);

	if (curl == nullptr) {
		throw logic_error("RestViewTest::executeRequest()  curl == nullptr");
	}

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlStringWriter::writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.curlStringWriter);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CurlHeaderWriter::headerFunction);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.curlHeaderWriter);

	if (!headerField.empty()) {
		header = curl_slist_append(header, headerField.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
	}

	if (!body.empty()) {
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlStringReader::readFunction);
		curl_easy_setopt(curl, CURLOPT_READDATA, &curlStringReader);
	}

	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.responseCode);
	curl_easy_cleanup(curl);
	curl_slist_free_all(header);

    return response;
}

RestViewTest::Response RestViewTest::executeRequest(const string& method, const string&  url) {
	return __executeRequest__(method, url, "", "");
}

RestViewTest::Response RestViewTest::executeRequest(const string& method, const string&  url, const string& customHeader) {
	return __executeRequest__(method, url, "", customHeader);
}

RestViewTest::Response RestViewTest::executeRequest(const string& method, const string&  url, const string& body, const string& contentType) {
	return __executeRequest__(method, url, body, "Content-Type: " + contentType);
}

void RestViewTest::checkResponseWithoutBody(const RestViewTest::Response& response, long statusCode) {
	EXPECT_THAT(response.responseCode, Eq(statusCode));
	EXPECT_THAT(response.curlStringWriter.getText(), IsEmpty());
	EXPECT_THAT(response.curlHeaderWriter.getHeaders(), Not(Contains(HasSubstr("Content-Type:"))));
}

void RestViewTest::checkResponseWithBody(const RestViewTest::Response& response, long statusCode, const string& contentType) {
	EXPECT_THAT(response.responseCode, Eq(statusCode));
	EXPECT_THAT(response.curlStringWriter.getText(), Not(IsEmpty()));
	EXPECT_THAT(response.curlHeaderWriter.getHeaders(), Contains("Content-Type: " + contentType + "\r\n"));
}

void RestViewTest::checkResponseWithBody(const RestViewTest::Response& response, long expectedStatusCode, const string& expectedContentType, const std::string& expectedBody) {
	EXPECT_THAT(response.responseCode, Eq(expectedStatusCode));
	EXPECT_THAT(response.curlStringWriter.getText(), Eq(expectedBody));
	EXPECT_THAT(response.curlHeaderWriter.getHeaders(), Contains("Content-Type: " + expectedContentType + "\r\n"));
}

void RestViewTest::checkErrorResponse(const RestViewTest::Response& response, long statusCode, const string& contentType) {
	checkResponseWithBody(response, statusCode, contentType);
}
