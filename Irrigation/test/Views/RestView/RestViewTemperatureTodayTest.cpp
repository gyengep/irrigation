#include "RestViewTemperatureTodayTest.h"
#include "Request.h"
#include "Temperature/TemperatureException.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const LocalDateTime RestViewTemperatureTodayTest::now(2021, 5, 29, 12, 21, 22);
const LocalDateTime RestViewTemperatureTodayTest::from(2021, 5, 29, 0, 0, 0);
const LocalDateTime RestViewTemperatureTodayTest::to(2021, 5, 29, 23, 59, 59);

const std::string RestViewTemperatureTodayTest::styleSheetFile("/temperature-today.xsl");

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureTodayTest::SetUp() {

	RestViewTemperatureTestBase::SetUp();

	EXPECT_CALL(*mockTimefunc, getTime()).
			WillRepeatedly(Return(now.toRawTime()));
}

void RestViewTemperatureTodayTest::TearDown() {
	RestViewTemperatureTestBase::TearDown();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureTodayTest, GET) {
	const TemperatureHistoryDto sampleHistoryDto("Sat, 29 May 2021 00:00:00 +0200", "Sat, 29 May 2021 12:21:22 +0200", "C", 20, 25, 23);
	const TemperatureForecastDto sampleForecastDto("Sat, 29 May 2021 12:21:22 +0200", "Sat, 29 May 2021 23:59:59 +0200", "C", 30, 35);
	const std::string sampleXml(
			"<temperature>"
				"<unit>C</unit>"
				"<historical>"
					"<value-min>20</value-min>"
					"<value-max>25</value-max>"
					"<value-avg>23</value-avg>"
					"<datetime-from>Sat, 29 May 2021 00:00:00 +0200</datetime-from>"
					"<datetime-to>Sat, 29 May 2021 12:21:22 +0200</datetime-to>"
				"</historical>"
				"<forecast>"
					"<value-min>30</value-min>"
					"<value-max>35</value-max>"
					"<datetime-from>Sat, 29 May 2021 12:21:22 +0200</datetime-from>"
					"<datetime-to>Sat, 29 May 2021 23:59:59 +0200</datetime-to>"
				"</forecast>"
			"</temperature>"
	);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, now, defaultDateTimeFormat)).
			Times(2).
			WillRepeatedly(Return(sampleHistoryDto));

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(now, to, defaultDateTimeFormat)).
			Times(2).
			WillRepeatedly(Return(sampleForecastDto));

	checkResponse_200_OK(
			GET(createTemperatureTodayUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureTodayUrl()),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
		);
}

TEST_F(RestViewTemperatureTodayTest, GET_WithDatetimeFormat1) {
	const std::string format("abc");
	const TemperatureHistoryDto sampleHistoryDto("abc", "abc", "C", 20, 25, 23);
	const TemperatureForecastDto sampleForecastDto("abc", "abc", "C", 30, 35);
	const std::string sampleXml(
			"<temperature>"
				"<unit>C</unit>"
				"<historical>"
					"<value-min>20</value-min>"
					"<value-max>25</value-max>"
					"<value-avg>23</value-avg>"
					"<datetime-from>abc</datetime-from>"
					"<datetime-to>abc</datetime-to>"
				"</historical>"
				"<forecast>"
					"<value-min>30</value-min>"
					"<value-max>35</value-max>"
					"<datetime-from>abc</datetime-from>"
					"<datetime-to>abc</datetime-to>"
				"</forecast>"
			"</temperature>"
	);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, now, format)).
			Times(1).
			WillOnce(Return(sampleHistoryDto));

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(now, to, format)).
			Times(1).
			WillOnce(Return(sampleForecastDto));

	checkResponse_200_OK(
			GET(createTemperatureTodayUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
	);
}

TEST_F(RestViewTemperatureTodayTest, GET_WithDatetimeFormat2) {
	const std::string format("%H:%M:%S");
	const TemperatureHistoryDto sampleHistoryDto("00:00:00", "12:21:22", "C", 20, 25, 23);
	const TemperatureForecastDto sampleForecastDto("12:21:22", "23:59:59", "C", 30, 35);
	const std::string sampleXml(
			"<temperature>"
				"<unit>C</unit>"
				"<historical>"
					"<value-min>20</value-min>"
					"<value-max>25</value-max>"
					"<value-avg>23</value-avg>"
					"<datetime-from>00:00:00</datetime-from>"
					"<datetime-to>12:21:22</datetime-to>"
				"</historical>"
				"<forecast>"
					"<value-min>30</value-min>"
					"<value-max>35</value-max>"
					"<datetime-from>12:21:22</datetime-from>"
					"<datetime-to>23:59:59</datetime-to>"
				"</forecast>"
			"</temperature>"
	);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, now, format)).
			Times(1).
			WillOnce(Return(sampleHistoryDto));

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(now, to, format)).
			Times(1).
			WillOnce(Return(sampleForecastDto));

	checkResponse_200_OK(
			GET(createTemperatureTodayUrl("datetime-format=" + format)),
			prependXmlAndStyleSheetHeader(sampleXml, styleSheetFile)
	);
}

TEST_F(RestViewTemperatureTodayTest, GET_HistoryNotFound) {
	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, now, defaultDateTimeFormat)).
			Times(1).
			WillOnce(Throw(TemperatureException("")));

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(now, to, defaultDateTimeFormat)).
			Times(0);

	checkResponse_404_Not_Found(
			GET(createTemperatureTodayUrl())
		);
}

TEST_F(RestViewTemperatureTodayTest, GET_ForecastNotFound) {
	const TemperatureHistoryDto sampleHistoryDto("00:00:00", "12:21:22", "C", 20, 25, 23);

	EXPECT_CALL(*mockTemperatureHistory, toTemperatureHistoryDto(from, now, defaultDateTimeFormat)).
			Times(1).
			WillOnce(Return(sampleHistoryDto));

	EXPECT_CALL(*mockTemperatureForecast, toTemperatureForecastDto(now, to, defaultDateTimeFormat)).
			Times(1).
			WillOnce(Throw(TemperatureException("")));

	checkResponse_404_Not_Found(
			GET(createTemperatureTodayUrl())
		);
}
