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


string RestViewTest::createTemperatureUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature");
	} else {
		return createUrl("/temperature") + "?" + requestParameters;
	}
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
	const auto now = LocalDateTime::create(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);


	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl());
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString("Tue, 22 Jun 2021 23:36:57 +0200", 28));
}

TEST_F(RestViewTest, getTemperatureWithTimeFormat) {
	const auto now = LocalDateTime::create(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl("datetime-format=%H:%M"));
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString("23:36", 28));
}

TEST_F(RestViewTest, getTemperatureAcceptable) {
	const auto now = LocalDateTime::create(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString("Tue, 22 Jun 2021 23:36:57 +0200", 28));
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
			XmlTemperatureWriter().forecastToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTemperatureForecastWithDatetimeFormat) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl("datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString("2021-05-29", "2021-05-29", values)
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
			XmlTemperatureWriter().forecastToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTemperatureForecastTodayWithDatetimeFormat) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureForecastUrl("day=today&datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().forecastToString("2021-05-29", "2021-05-29", values)
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
			XmlTemperatureWriter().forecastToString("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 23:59:59 +0200", values)
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
			XmlTemperatureWriter().forecastToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
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
			XmlTemperatureWriter().historyToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryWithDatetimeFormat) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl("datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString("2021-05-29", "2021-05-29", values)
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
			XmlTemperatureWriter().historyToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTemperatureHistoryTodayWithDatetimeFormat) {
	const auto now = LocalDateTime::create(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawtime()));

	DateTime::setTimefunc(mockTimefunc);

	const auto from = LocalDateTime::create(2021, 5, 29, 0, 0, 0);
	const auto to = LocalDateTime::create(2021, 5, 29, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureHistoryUrl("day=today&datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().historyToString("2021-05-29", "2021-05-29", values)
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
			XmlTemperatureWriter().historyToString("Fri, 28 May 2021 00:00:00 +0200", "Fri, 28 May 2021 23:59:59 +0200", values)
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
			XmlTemperatureWriter().historyToString("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 23:59:59 +0200", values)
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
