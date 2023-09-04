#include "RestViewCurrentTemperatureTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RestViewTestSamples::CurrentTemperatureSample RestViewCurrentTemperatureTest::sample;
const std::string RestViewCurrentTemperatureTest::styleSheetFile("/temperature-current.xsl");

///////////////////////////////////////////////////////////////////////////////

std::string RestViewCurrentTemperatureTest::createCurrentTemperatureUrl(const std::string& requestParameters) {
	std::string result = createUrl("/temperature/current");

	if (false == requestParameters.empty()) {
		result.append("?" + requestParameters);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewCurrentTemperatureTest, get) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDTO(defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createCurrentTemperatureUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewCurrentTemperatureTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDTO(defaultDateTimeFormat)).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createCurrentTemperatureUrl()),
			prependXmlAndStyleSheetHeader(sample.getXml(), styleSheetFile)
		);
}

TEST_F(RestViewCurrentTemperatureTest, get_WithDatetimeFormat1) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDTO("abc")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createCurrentTemperatureUrl("datetime-format=abc"));
}

TEST_F(RestViewCurrentTemperatureTest, get_WithDatetimeFormat2) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDTO("%a %b")).Times(1).WillOnce(Return(sample.getDto()));
	GET(createCurrentTemperatureUrl("datetime-format=%a %b"));
}

TEST_F(RestViewCurrentTemperatureTest, get_NotFound) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDTO(defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createCurrentTemperatureUrl())
		);
}

TEST_F(RestViewCurrentTemperatureTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createCurrentTemperatureUrl())
		);
}
