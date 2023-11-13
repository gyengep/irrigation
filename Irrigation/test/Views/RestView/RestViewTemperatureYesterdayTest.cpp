#include "RestViewTemperatureYesterdayTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RestViewTestSamples::TemperatureYesterdaySample RestViewTemperatureYesterdayTest::sample;
const std::string RestViewTemperatureYesterdayTest::styleSheetFile("/temperature-yesterday.xsl");

///////////////////////////////////////////////////////////////////////////////

std::string RestViewTemperatureYesterdayTest::createTemperatureYesterdayUrl(const std::string& requestParameters) {
	std::string result = createUrl("/temperature/yesterday");

	if (false == requestParameters.empty()) {
		result.append("?" + requestParameters);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureYesterdayTest, GET) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_WithDatetimeFormat1) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getTo(), "abc")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureYesterdayUrl("datetime-format=abc"));
}

TEST_F(RestViewTemperatureYesterdayTest, GET_WithDatetimeFormat2) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getTo(), "%a %b")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureYesterdayUrl("datetime-format=%a %b"));
}

TEST_F(RestViewTemperatureYesterdayTest, GET_NotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(sample.getFrom(), sample.getTo(), defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureYesterdayUrl())
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureYesterdayUrl())
		);
}
