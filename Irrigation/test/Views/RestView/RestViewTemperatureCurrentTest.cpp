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
	EXPECT_CALL(*mockCurrentTemperature, toTemperatureCurrentDTO(defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createTemperatureCurrentUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockCurrentTemperature, toTemperatureCurrentDTO(defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureCurrentUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_WithDatetimeFormat) {
	EXPECT_CALL(*mockCurrentTemperature, toTemperatureCurrentDTO("abc")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureCurrentUrl("datetime-format=abc"));

	EXPECT_CALL(*mockCurrentTemperature, toTemperatureCurrentDTO("%a %b")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createTemperatureCurrentUrl("datetime-format=%a %b"));
}

TEST_F(RestViewTemperatureCurrentTest, get_NotFound) {
	EXPECT_CALL(*mockCurrentTemperature, toTemperatureCurrentDTO(defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureCurrentUrl())
		);
}

TEST_F(RestViewTemperatureCurrentTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureCurrentUrl())
		);
}
