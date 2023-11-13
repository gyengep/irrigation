#include "RestViewTemperatureTomorrowTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RestViewTestSamples::TemperatureTomorrowSample RestViewTemperatureTomorrowTest::sample;
const std::string RestViewTemperatureTomorrowTest::styleSheetFile("/temperature-tomorrow.xsl");

///////////////////////////////////////////////////////////////////////////////

std::string RestViewTemperatureTomorrowTest::createTemperatureTomorrowUrl(const std::string& requestParameters) {
	std::string result = createUrl("/temperature/tomorrow");

	if (false == requestParameters.empty()) {
		result.append("?" + requestParameters);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureTomorrowTest, GET) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_WithDatetimeFormat1) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast,toTemperatureForecastDto(sample.getFrom(), sample.getTo(), "abc")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureTomorrowUrl("datetime-format=abc"));
}

TEST_F(RestViewTemperatureTomorrowTest, GET_WithDatetimeFormat2) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast,toTemperatureForecastDto(sample.getFrom(), sample.getTo(), "%a %b")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureTomorrowUrl("datetime-format=%a %b"));
}

TEST_F(RestViewTemperatureTomorrowTest, GET_NotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureTomorrowUrl())
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureTomorrowUrl())
		);
}
