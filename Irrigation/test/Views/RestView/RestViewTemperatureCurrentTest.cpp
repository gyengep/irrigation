#include "RestViewTemperatureCurrentTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const Dto2XmlTest::TemperatureCurrentSample RestViewTemperatureCurrentTest::sample;
const std::string RestViewTemperatureCurrentTest::styleSheetFile("/temperature-current.xsl");

///////////////////////////////////////////////////////////////////////////////

std::string RestViewTemperatureCurrentTest::createTemperatureCurrentUrl(const std::string& requestParameters) {
	std::string result = createUrl("/temperature/current");

	if (false == requestParameters.empty()) {
		result.append("?" + requestParameters);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureCurrentTest, get) {
	EXPECT_CALL(*mockTimefunc, getTime()).Times(AnyNumber()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(sample.getValue()));

	checkResponse_200_OK(
			GET(createTemperatureCurrentUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockTimefunc, getTime()).Times(AnyNumber()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Return(sample.getValue()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureCurrentUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_WithDatetimeFormat) {
	EXPECT_CALL(*mockTimefunc, getTime()).Times(AnyNumber()).WillRepeatedly(Return(sample.getNow().toRawTime()));
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(2).WillRepeatedly(Return(sample.getValue()));

	checkResponse_200_OK(
			GET(createTemperatureCurrentUrl(sample.getDateTimeFormatAndXml(0).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(0).second, styleSheetFile)
		);

	checkResponse_200_OK(
			GET(createTemperatureCurrentUrl(sample.getDateTimeFormatAndXml(1).first)),
			prependXmlAndStyleSheetHeader(sample.getDateTimeFormatAndXml(1).second, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_NotFound) {
	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureCurrentUrl())
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureCurrentUrl())
		);
}
