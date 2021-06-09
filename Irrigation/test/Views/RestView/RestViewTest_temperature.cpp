#include "RestViewTest.h"
#include "Mocks/MockCurrentTemperature.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTimefunc.h"
#include "Model/IrrigationDocument.h"
#include "Temperature/TemperatureException.h"
#include "Views/RestView/RestView.h"
#include "Views/RestView/XmlTemperatureWriter.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createTemperatureUrl() {
	return createUrl("/temperature");
}

string RestViewTest::createTemperatureForecastUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature/forecast");
	} else {
		return createUrl("/temperature/forecast") + "?" + requestParameters;
	}
}

string RestViewTest::createTemperatureHistoryUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature/historical");
	} else {
		return createUrl("/temperature/historical") + "?" + requestParameters;
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getTemperature) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl());
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString(28));
}

TEST_F(RestViewTest, getTemperatureAcceptable) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString(28));
}

TEST_F(RestViewTest, getTemperatureNotFound) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTemperatureNotAcceptable) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(0);

	const Response response = executeRequest("GET", createTemperatureUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getTemperatureForecast) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl());
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureForecastToday) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl("day=today"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureForecastTomorrow) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 30, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 30, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl("day=tomorrow"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureForecastInvalidDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(_, _)).Times(0);

	const Response response = executeRequest("GET", createTemperatureForecastUrl("day=abcd"));
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, getTemperatureForecastAcceptable) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureForecastNotFound) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);
	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureForecastUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTemperatureForecastNotAcceptable) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(0);

	const Response response = executeRequest("GET", createTemperatureForecastUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getTemperatureHistory) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl());
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryToday) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl("day=today"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryYesterday) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 28, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 28, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl("day=yesterday"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryInvalidDay) {
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(_, _)).Times(0);

	const Response response = executeRequest("GET", createTemperatureHistoryUrl("day=abcd"));
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, getTemperatureHistoryAcceptable) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString(values, from, to)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryNotFound) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTemperatureHistoryNotAcceptable) {
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(_, _)).Times(0);

	const Response response = executeRequest("GET", createTemperatureHistoryUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}
