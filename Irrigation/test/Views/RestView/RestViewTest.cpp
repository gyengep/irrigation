#include "RestViewTest.h"
#include "Request.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::SetUp() {

	mockIrrigationDocument = std::make_shared<StrictMock<MockIrrigationDocument>>();
	mockCurrentTemperature = std::make_shared<StrictMock<MockCurrentTemperature>>();
	mockTemperatureHistory = std::make_shared<StrictMock<MockTemperatureHistory>>();
	mockTemperatureForecast = std::make_shared<StrictMock<MockTemperatureForecast>>();
	mockShutdownManager = std::make_shared<StrictMock<MockShutdownManager>>();

	restView = std::make_shared<RestView>(*mockIrrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		);

	restView->initialize();
}

void RestViewTest::TearDown() {
	restView->terminate();
}

std::string RestViewTest::createUrl(const std::string& path) {
	if ('/' != path[0]) {
		throw std::logic_error("The path has to start with '/'");
	}

	std::ostringstream o;
	o << "http://localhost:" << port << "/api/v1" << path;

	return o.str();
}

AAA::Response RestViewTest::DELETE(const std::string& url) {
	return AAA::Request("DELETE", url).
		execute();
}

AAA::Response RestViewTest::GET(const std::string& url) {
	return AAA::Request("GET", url).
		execute();
}

AAA::Response RestViewTest::GET_Accept_Xml(const std::string& url) {
	return AAA::Request("GET", url).
		addHeader("Accept: application/xml").
		execute();
}

AAA::Response RestViewTest::GET_Accept_Json(const std::string& url) {
	return AAA::Request("GET", url).
		addHeader("Accept: application/json").
		execute();
}

AAA::Response RestViewTest::PATCH_ContentType_Xml(const std::string& url, const std::string& text) {
	return AAA::Request("PATCH", url).
		addHeader("Content-Type: application/xml").
		setBody(text).
		execute();
}

AAA::Response RestViewTest::PATCH_ContentType_Json(const std::string& url, const std::string& text) {
	return AAA::Request("PATCH", url).
		addHeader("Content-Type: application/json").
		setBody(text).
		execute();
}

AAA::Response RestViewTest::POST_ContentType_Xml(const std::string& url, const std::string& text) {
	return AAA::Request("POST", url).
		addHeader("Content-Type: application/xml").
		setBody(text).
		execute();
}

AAA::Response RestViewTest::POST_ContentType_Json(const std::string& url, const std::string& text) {
	return AAA::Request("POST", url).
		addHeader("Content-Type: application/json").
		setBody(text).
		execute();
}

std::string RestViewTest::stripXml(const std::string& text) {
	std::ostringstream oss;
    std::istringstream f(text);
    std::string line;
    while (std::getline(f, line)) {
    	line.erase(0, line.find_first_not_of("\t"));
   		oss << line;
    }

	return oss.str();
}

std::string RestViewTest::prependXmlHeader(const std::string& xml) {
	static const std::string xmlHeader(R"(<?xml version="1.0"?>)");
	return xmlHeader + xml;
}

std::string RestViewTest::prependXmlAndStyleSheetHeader(const std::string& xml, const std::string& xslFile) {
	const std::string styleSheetPI(R"(<?xml-stylesheet type="text/xsl" href=")" + xslFile + R"("?>)");
	return prependXmlHeader(styleSheetPI + xml);
}

void RestViewTest::checkResponse_200_OK(const AAA::Response& response, const std::string& expectedXml) {
	EXPECT_THAT(response.getResponseCode(), Eq(200));
	EXPECT_THAT(stripXml(response.getBody()), Eq(expectedXml));
	EXPECT_THAT(response.getHeaders(), Contains("Content-Type: application/xml\r\n"));
}

void RestViewTest::checkResponse_201_Created(const AAA::Response& response, const std::string& expectedLocation) {
	EXPECT_THAT(response.getResponseCode(), Eq(201));
	EXPECT_THAT(response.getBody(), IsEmpty());
	EXPECT_THAT(response.getHeaders(), Not(Contains(HasSubstr("Content-Type:"))));
	EXPECT_THAT(response.getHeaders(), Contains("Location: " + expectedLocation + "\r\n"));
}

void RestViewTest::checkResponse_204_No_Content(const AAA::Response& response) {
	EXPECT_THAT(response.getResponseCode(), Eq(204));
	EXPECT_THAT(response.getBody(), IsEmpty());
	EXPECT_THAT(response.getHeaders(), Not(Contains(HasSubstr("Content-Type:"))));
}

void RestViewTest::checkResponse_400_Bad_Request(const AAA::Response& response) {
	checkResponseIsError(400, response);
}

void RestViewTest::checkResponse_404_Not_Found(const AAA::Response& response) {
	checkResponseIsError(404, response);
}

void RestViewTest::checkResponse_405_Method_Not_Allowed(const AAA::Response& response) {
	checkResponseIsError(405, response);
}

void RestViewTest::checkResponse_406_Not_Acceptable(const AAA::Response& response) {
	checkResponseIsError(406, response);
}

void RestViewTest::checkResponse_415_Unsupported_Media_Type(const AAA::Response& response) {
	checkResponseIsError(415, response);
}

void RestViewTest::checkResponseIsError(long responseCode, const AAA::Response& response) {
	EXPECT_THAT(response.getResponseCode(), Eq(responseCode));
	EXPECT_THAT(response.getBody(), Not(IsEmpty()));
	EXPECT_THAT(response.getHeaders(), Contains("Content-Type: application/xml\r\n"));
}
