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
