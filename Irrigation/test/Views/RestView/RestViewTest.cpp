#include "RestViewTest.h"
#include "Request.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const std::string RestViewTest::defaultDateTimeFormat("%a, %d %b %G %H:%M:%S %z");
const uint16_t RestViewTest::port = 8080;

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::SetUp() {

	mockIrrigationDocument = std::make_shared<StrictMock<MockIrrigationDocument>>();
	mockCurrentTemperature = std::make_shared<StrictMock<MockCurrentTemperature>>();
	mockTemperatureHistory = std::make_shared<StrictMock<MockTemperatureHistory>>();
	mockTemperatureForecast = std::make_shared<StrictMock<MockTemperatureForecast>>();
	mockShutdownManager = std::make_shared<StrictMock<MockShutdownManager>>();
	mockFileWriter = std::make_shared<StrictMock<MockFileWriter>>();
	mockFileWriterFactory = std::make_shared<StrictMock<MockFileWriterFactory>>();

	ON_CALL(*mockFileWriterFactory, create(_)).WillByDefault(Return(mockFileWriter));
	EXPECT_CALL(*mockFileWriter, write(_)).Times(1);
	EXPECT_CALL(*mockFileWriterFactory, create(_)).Times(1);

	restView = std::make_shared<RestView>(*mockIrrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			mockFileWriterFactory,
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

void RestViewTest::checkResponse_422_Unprocessable_Content(const AAA::Response& response) {
	checkResponseIsError(422, response);
}

void RestViewTest::checkResponseIsError(long responseCode, const AAA::Response& response) {
	EXPECT_THAT(response.getResponseCode(), Eq(responseCode));
	EXPECT_THAT(response.getBody(), Not(IsEmpty()));
	EXPECT_THAT(response.getHeaders(), Contains("Content-Type: application/xml\r\n"));
}
