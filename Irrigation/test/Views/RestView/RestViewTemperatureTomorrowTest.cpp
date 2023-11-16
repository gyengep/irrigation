#include "RestViewTemperatureTomorrowTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const LocalDateTime RestViewTemperatureTomorrowTest::now(2021, 5, 29, 12, 21, 22);
const LocalDateTime RestViewTemperatureTomorrowTest::from(2021, 5, 30, 0, 0, 0);
const LocalDateTime RestViewTemperatureTomorrowTest::to(2021, 5, 30, 23, 59, 59);

const std::string RestViewTemperatureTomorrowTest::styleSheetFile("/temperature-tomorrow.xsl");

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureTomorrowTest::SetUp() {

	RestViewTemperatureTestBase::SetUp();

	EXPECT_CALL(*mockTimefunc, getTime()).
			WillRepeatedly(Return(now.toRawTime()));
}

void RestViewTemperatureTomorrowTest::TearDown() {
	RestViewTemperatureTestBase::TearDown();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureTomorrowTest, GET) {
	const TemperatureForecastDto sampleDto("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 23:59:59 +0200", "C", 25, 30);
	const std::string sampleXml =
	"<temperature>"
		"<unit>C</unit>"
		"<forecast>"
			"<value-min>25</value-min>"
			"<value-max>30</value-max>"
			"<datetime-from>Sun, 30 May 2021 00:00:00 +0200</datetime-from>"
			"<datetime-to>Sun, 30 May 2021 23:59:59 +0200</datetime-to>"
		"</forecast>"
	"</temperature>";

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(from, to, defaultDateTimeFormat)).
			Times(2).
			WillRepeatedly(Return(sampleDto));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureTomorrowUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_WithDatetimeFormat1) {
	const std::string format("abc");
	const TemperatureForecastDto sampleDto("abc", "abc", "C", 12, 15);
	const std::string sampleXml(
	"<temperature>"
		"<unit>C</unit>"
		"<forecast>"
			"<value-min>12</value-min>"
			"<value-max>15</value-max>"
			"<datetime-from>abc</datetime-from>"
			"<datetime-to>abc</datetime-to>"
		"</forecast>"
	"</temperature>");

	EXPECT_CALL(*mockTemperatureForecast,toTemperatureForecastDto(from, to, format)).
			Times(1).
			WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_WithDatetimeFormat2) {
	const std::string format("%H:%M:%S");
	const TemperatureForecastDto sampleDto("00:00:00", "23:59:59", "C", 30, 31);
	const std::string sampleXml(
	"<temperature>"
		"<unit>C</unit>"
		"<forecast>"
			"<value-min>30</value-min>"
			"<value-max>31</value-max>"
			"<datetime-from>00:00:00</datetime-from>"
			"<datetime-to>23:59:59</datetime-to>"
		"</forecast>"
	"</temperature>");

	EXPECT_CALL(*mockTemperatureForecast,toTemperatureForecastDto(from, to, format)).
			Times(1).
			WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createTemperatureTomorrowUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTomorrowTest, GET_NotFound) {
	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(from, to, defaultDateTimeFormat)).
			Times(1).
			WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureTomorrowUrl())
		);
}
