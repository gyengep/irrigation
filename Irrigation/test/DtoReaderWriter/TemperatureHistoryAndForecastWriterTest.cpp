#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/TemperatureHistoryAndForecastSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryAndForecastWriterTest, save) {
	for (const auto& temperatureHistoryAndForecastSample : DtoReaderWriterTestSamples::TemperatureHistoryAndForecastSampleList()) {
		const std::string actualXml = XmlWriter(false).save(
				temperatureHistoryAndForecastSample.getDto().first,
				temperatureHistoryAndForecastSample.getDto().second,
				"MyStyleSheetFile"
			);

		const std::string expectedXml = temperatureHistoryAndForecastSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}

TEST(TemperatureHistoryAndForecastWriterTest, saveWithDifferentUnit) {
	const std::string dateTimeFrom("2021-05-01");
	const std::string dateTimeTo("2021-05-02");
	const float minTemperature = 20.f;
	const float maxTemperature = 25.f;
	const float avgTemperature = 30.f;

	EXPECT_THROW(XmlWriter(false).save(
				TemperatureHistoryDTO(dateTimeFrom, dateTimeTo, "C1", minTemperature, maxTemperature, avgTemperature),
				TemperatureForecastDTO(dateTimeFrom, dateTimeTo, "C2", minTemperature, maxTemperature),
				"MyStyleSheetFile"
			),
			std::logic_error
		);

	EXPECT_NO_THROW(XmlWriter(false).save(
				TemperatureHistoryDTO(dateTimeFrom, dateTimeTo, "C1", minTemperature, maxTemperature, avgTemperature),
				TemperatureForecastDTO(dateTimeFrom, dateTimeTo, "C1", minTemperature, maxTemperature),
				"MyStyleSheetFile"
			)
		);
}
