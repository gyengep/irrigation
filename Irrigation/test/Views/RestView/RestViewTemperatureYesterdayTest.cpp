#include "RestViewTemperatureYesterdayTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const Dto2XmlTest::TemperatureYesterdaySample RestViewTemperatureYesterdayTest::sample;
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

TEST_F(RestViewTemperatureYesterdayTest, get) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Return(sample.getHistoryValues()));

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Return(sample.getHistoryValues()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, get_WithDatetimeFormat) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(sample.getFrom(), sample.getTo())).Times(2).WillRepeatedly(Return(sample.getHistoryValues()));

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl(sample.getDateTimeFormatAndXml(0).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(0).second, styleSheetFile)
		);

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl(sample.getDateTimeFormatAndXml(1).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(1).second, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, get_NotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureYesterdayUrl())
		);
}

TEST_F(RestViewTemperatureYesterdayTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureYesterdayUrl())
		);
}
