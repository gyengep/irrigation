#include "RestViewTemperatureYesterdayTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const LocalDateTime RestViewTemperatureYesterdayTest::now(2021, 5, 29, 12, 21, 22);
const LocalDateTime RestViewTemperatureYesterdayTest::from(2021, 5, 28, 0, 0, 0);
const LocalDateTime RestViewTemperatureYesterdayTest::to(2021, 5, 28, 23, 59, 59);

const std::string RestViewTemperatureYesterdayTest::styleSheetFile("/temperature-yesterday.xsl");

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureYesterdayTest::SetUp() {

	RestViewTemperatureTestBase::SetUp();

	EXPECT_CALL(*mockTimefunc, getTime()).
			WillRepeatedly(Return(now.toRawTime()));
}

void RestViewTemperatureYesterdayTest::TearDown() {
	RestViewTemperatureTestBase::TearDown();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureYesterdayTest, GET) {
	const TemperatureHistoryDto sampleDto("Mon, 21 Jun 2021 00:00:00 +0200", "Mon, 21 Jun 2021 23:59:59 +0200", "celsius", 25, 30, 28);
	const std::string sampleXml(
			"<temperature>"
				"<unit>celsius</unit>"
				"<historical>"
					"<value-min>25</value-min>"
					"<value-max>30</value-max>"
					"<value-avg>28</value-avg>"
					"<datetime-from>Mon, 21 Jun 2021 00:00:00 +0200</datetime-from>"
					"<datetime-to>Mon, 21 Jun 2021 23:59:59 +0200</datetime-to>"
				"</historical>"
			"</temperature>"
		);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, to, defaultDateTimeFormat)).
			Times(2).
			WillRepeatedly(Return(sampleDto));


	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureYesterdayUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_WithDatetimeFormat1) {
	const std::string format("abc");
	const TemperatureHistoryDto sampleDto("abc", "abc", "celsius", 25, 30, 28);
	const std::string sampleXml(
			"<temperature>"
				"<unit>celsius</unit>"
				"<historical>"
					"<value-min>25</value-min>"
					"<value-max>30</value-max>"
					"<value-avg>28</value-avg>"
					"<datetime-from>abc</datetime-from>"
					"<datetime-to>abc</datetime-to>"
				"</historical>"
			"</temperature>"
		);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, to, format)).
			Times(1).
			WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_WithDatetimeFormat2) {
	const std::string format("%H:%M:%S");
	const TemperatureHistoryDto sampleDto("00:00:00", "23:59:59", "celsius", 25, 30, 28);
	const std::string sampleXml(
			"<temperature>"
				"<unit>celsius</unit>"
				"<historical>"
					"<value-min>25</value-min>"
					"<value-max>30</value-max>"
					"<value-avg>28</value-avg>"
					"<datetime-from>00:00:00</datetime-from>"
					"<datetime-to>23:59:59</datetime-to>"
				"</historical>"
			"</temperature>"
		);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, to, format)).
			Times(1).
			WillOnce(Return(sampleDto));

	checkResponse_200_OK(
			GET(createTemperatureYesterdayUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureYesterdayTest, GET_NotFound) {
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, to, defaultDateTimeFormat)).
			Times(1).
			WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureYesterdayUrl())
		);
}
