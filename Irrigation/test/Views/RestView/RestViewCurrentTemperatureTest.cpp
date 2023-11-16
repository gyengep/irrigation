#include "RestViewCurrentTemperatureTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const std::string RestViewCurrentTemperatureTest::styleSheetFile("/temperature-current.xsl");

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewCurrentTemperatureTest, GET) {
	const CurrentTemperatureDto sampleDto("Tue, 22 Jun 2021 23:36:57 +0200", "celsius", 28.1);
	const std::string sampleXml(
			"<temperature>"
				"<value>28</value>"
				"<datetime>Tue, 22 Jun 2021 23:36:57 +0200</datetime>"
				"<unit>celsius</unit>"
			"</temperature>"
		);

	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDto(defaultDateTimeFormat)).
			Times(2).
			WillRepeatedly(Return(sampleDto));

	checkResponse_200_OK(
			GET(createCurrentTemperatureUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createCurrentTemperatureUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewCurrentTemperatureTest, GET_WithDatetimeFormat1) {
	const std::string format("abc");
	const CurrentTemperatureDto sampleDto("abc", "celsius", 28.1);
	const std::string sampleXml(
			"<temperature>"
				"<value>28</value>"
				"<datetime>abc</datetime>"
				"<unit>celsius</unit>"
			"</temperature>"
		);

	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDto(format)).
			Times(1).
			WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createCurrentTemperatureUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewCurrentTemperatureTest, GET_WithDatetimeFormat2) {
	const std::string format("%H:%M:%S");
	const CurrentTemperatureDto sampleDto("23:36:57", "celsius", 28.1);
	const std::string sampleXml(
			"<temperature>"
				"<value>28</value>"
				"<datetime>23:36:57</datetime>"
				"<unit>celsius</unit>"
			"</temperature>"
		);

	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDto(format)).Times(1).WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createCurrentTemperatureUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewCurrentTemperatureTest, GET_NotFound) {
	EXPECT_CALL(*mockCurrentTemperature, toCurrentTemperatureDto(defaultDateTimeFormat)).Times(1).WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createCurrentTemperatureUrl())
		);
}
