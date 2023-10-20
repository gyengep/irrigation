#include "RestViewTemperatureTodayTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RestViewTestSamples::TemperatureTodaySample RestViewTemperatureTodayTest::sample;
const std::string RestViewTemperatureTodayTest::styleSheetFile("/temperature-today.xsl");

///////////////////////////////////////////////////////////////////////////////

std::string RestViewTemperatureTodayTest::createTemperatureTodayUrl(const std::string& requestParameters) {
	std::string result = createUrl("/temperature/today");

	if (false == requestParameters.empty()) {
		result.append("?" + requestParameters);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureTodayTest, get) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto().first));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto().second));

	checkResponse_200_OK(
			GET(createTemperatureTodayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTodayTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto().first));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto().second));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureTodayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTodayTest, get_WithDatetimeFormat1) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), "abc")).Times(1).WillOnce(Return(sample.getDto().first));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), "abc")).Times(1).WillOnce(Return(sample.getDto().second));
	GET(createTemperatureTodayUrl("datetime-format=abc"));
}

TEST_F(RestViewTemperatureTodayTest, get_WithDatetimeFormat2) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), "%a %b")).Times(1).WillOnce(Return(sample.getDto().first));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), "%a %b")).Times(1).WillOnce(Return(sample.getDto().second));
	GET(createTemperatureTodayUrl("datetime-format=%a %b"));
}

TEST_F(RestViewTemperatureTodayTest, get_HistoryNotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), defaultDateTimeFormat)).Times(0);

	checkResponse_404_Not_Found(
			GET(createTemperatureTodayUrl())
		);
}

TEST_F(RestViewTemperatureTodayTest, get_ForecastNotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getNow(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto().first));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getNow(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureTodayUrl())
		);
}

TEST_F(RestViewTemperatureTodayTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureTodayUrl())
		);
}
