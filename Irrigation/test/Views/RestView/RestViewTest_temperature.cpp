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
		return createUrl("/temperature/current");
	} else {
		return createUrl("/temperature/current") + "?" + requestParameters;
	}
}

string RestViewTest::createTemperatureTomorrowUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature/tomorrow");
	} else {
		return createUrl("/temperature/tomorrow") + "?" + requestParameters;
	}
}

string RestViewTest::createTemperatureTodayUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature/today");
	} else {
		return createUrl("/temperature/today") + "?" + requestParameters;
	}
}

string RestViewTest::createTemperatureYesterdayUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/temperature/yesterday");
	} else {
		return createUrl("/temperature/yesterday") + "?" + requestParameters;
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getTemperature) {
	const LocalDateTime now(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);


	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl());
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString("Tue, 22 Jun 2021 23:36:57 +0200", 28));
}

TEST_F(RestViewTest, getTemperatureWithTimeFormat) {
	const LocalDateTime now(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(28));

	const Response response = executeRequest("GET", createTemperatureUrl("datetime-format=%H:%M"));
	checkResponseWithBody(response, 200, "application/xml", XmlTemperatureWriter().currentToString("23:36", 28));
}

TEST_F(RestViewTest, getTemperatureAcceptable) {
	const LocalDateTime now(2021, 6, 22, 23, 36, 57);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

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

TEST_F(RestViewTest, getTomorrowTemperature) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to(2021, 5, 30, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureTomorrowUrl());
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().tomorrowToString("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTomorrowTemperatureWithDatetimeFormat) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to(2021, 5, 30, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureTomorrowUrl("datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().tomorrowToString("2021-05-30", "2021-05-30", values)
	);
}

TEST_F(RestViewTest, getTomorrowTemperatureAcceptable) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to(2021, 5, 30, 23, 59, 59);
	const auto values = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureTomorrowUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().tomorrowToString("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getTomorrowTemperatureNotFound) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);
	const LocalDateTime from(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to(2021, 5, 30, 23, 59, 59);

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(from, to)).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureTomorrowUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTomorrowTemperatureNotAcceptable) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(0);

	const Response response = executeRequest("GET", createTemperatureTomorrowUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getTodayTemperature) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 29, 0, 0, 0);
	const LocalDateTime to(2021, 5, 29, 23, 59, 59);
	const auto historyValues = TemperatureHistory::Values(20, 28, 23);
	const auto forecastValues = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, now)).Times(1).WillOnce(Return(historyValues));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(now, to)).Times(1).WillOnce(Return(forecastValues));

	const Response response = executeRequest("GET", createTemperatureTodayUrl());
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().todayToString(
					"Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 19:33:54 +0200", "Sat, 29 May 2021 23:59:59 +0200",
					historyValues, forecastValues
				)
	);
}

TEST_F(RestViewTest, getTodayTemperatureWithDatetimeFormat) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 29, 0, 0, 0);
	const LocalDateTime to(2021, 5, 29, 23, 59, 59);
	const auto historyValues = TemperatureHistory::Values(20, 28, 23);
	const auto forecastValues = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, now)).Times(1).WillOnce(Return(historyValues));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(now, to)).Times(1).WillOnce(Return(forecastValues));

	const Response response = executeRequest("GET", createTemperatureTodayUrl("datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().todayToString(
					"2021-05-29", "2021-05-29", "2021-05-29",
					historyValues, forecastValues
				)
	);
}

TEST_F(RestViewTest, getTodayTemperatureAcceptable) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 29, 0, 0, 0);
	const LocalDateTime to(2021, 5, 29, 23, 59, 59);
	const auto historyValues = TemperatureHistory::Values(20, 28, 23);
	const auto forecastValues = TemperatureForecast::Values(25, 30);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, now)).Times(1).WillOnce(Return(historyValues));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(now, to)).Times(1).WillOnce(Return(forecastValues));

	const Response response = executeRequest("GET", createTemperatureTodayUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().todayToString(
					"Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 19:33:54 +0200", "Sat, 29 May 2021 23:59:59 +0200",
					historyValues, forecastValues
				)
	);
}

TEST_F(RestViewTest, getTodayTemperature_HistoryNotFound) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);
	const LocalDateTime from(2021, 5, 29, 0, 0, 0);
	const LocalDateTime to(2021, 5, 29, 23, 59, 59);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, now)).Times(1).WillOnce(Throw(TemperatureException("")));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(now, to)).Times(0);

	const Response response = executeRequest("GET", createTemperatureTodayUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTodayTemperature_ForecastNotFound) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);
	const LocalDateTime from(2021, 5, 29, 0, 0, 0);
	const LocalDateTime to(2021, 5, 29, 23, 59, 59);
	const auto historyValues = TemperatureHistory::Values(25, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, now)).Times(1).WillOnce(Return(historyValues));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(now, to)).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureTodayUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTodayTemperatureNotAcceptable) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(0);

	const Response response = executeRequest("GET", createTemperatureTodayUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getYesterdayTemperature) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 28, 0, 0, 0);
	const LocalDateTime to(2021, 5, 28, 23, 59, 59);
	const auto values = TemperatureHistory::Values(25, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureYesterdayUrl());
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().yesterdayToString("Fri, 28 May 2021 00:00:00 +0200", "Fri, 28 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getYesterdayTemperatureWithDatetimeFormat) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 28, 0, 0, 0);
	const LocalDateTime to(2021, 5, 28, 23, 59, 59);
	const auto values = TemperatureHistory::Values(25, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureYesterdayUrl("datetime-format=%Y-%m-%d"));
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().yesterdayToString("2021-05-28", "2021-05-28", values)
	);
}

TEST_F(RestViewTest, getYesterdayTemperatureAcceptable) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 28, 0, 0, 0);
	const LocalDateTime to(2021, 5, 28, 23, 59, 59);
	const auto values = TemperatureHistory::Values(20, 30, 28);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Return(values));

	const Response response = executeRequest("GET", createTemperatureYesterdayUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml",
			XmlTemperatureWriter().yesterdayToString("Fri, 28 May 2021 00:00:00 +0200", "Fri, 28 May 2021 23:59:59 +0200", values)
	);
}

TEST_F(RestViewTest, getYesterdayTemperatureNotFound) {
	const LocalDateTime now(2021, 5, 29, 19, 33, 54);
	auto mockTimefunc = std::make_shared<MockTimefunc>();

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));

	DateTime::setTimefunc(mockTimefunc);

	const LocalDateTime from(2021, 5, 28, 0, 0, 0);
	const LocalDateTime to(2021, 5, 28, 23, 59, 59);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(from, to)).Times(1).WillOnce(Throw(TemperatureException("")));

	const Response response = executeRequest("GET", createTemperatureYesterdayUrl());
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getYesterdayTemperatureNotAcceptable) {
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(_, _)).Times(0);

	const Response response = executeRequest("GET", createTemperatureYesterdayUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}
