#include "XmlReaderWriterTest.h"
#include "Dto2Xml/TemperatureHistoryAndForecastSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryAndForecastWriterTest, save) {
	for (const auto& temperatureHistoryAndForecastSample : Dto2XmlTest::TemperatureHistoryAndForecastSampleList()) {
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
