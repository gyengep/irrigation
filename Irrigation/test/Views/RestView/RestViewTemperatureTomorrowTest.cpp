#include "RestViewTemperatureTomorrowTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const Dto2XmlTest::TemperatureTomorrowSample RestViewTemperatureTomorrowTest::sample;
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

TEST_F(RestViewTemperatureTomorrowTest, get) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Return(sample.getForecastValues()));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Return(sample.getForecastValues()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, get_WithDatetimeFormat) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(sample.getFrom(), sample.getTo())).Times(2).WillRepeatedly(Return(sample.getForecastValues()));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl(sample.getDateTimeFormatAndXml(0).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(0).second, styleSheetFile)
		);

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl(sample.getDateTimeFormatAndXml(1).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(1).second, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, get_NotFound) {
	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(sample.getFrom(), sample.getTo())).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureTomorrowUrl())
		);
}

TEST_F(RestViewTemperatureTomorrowTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureTomorrowUrl())
		);
}
