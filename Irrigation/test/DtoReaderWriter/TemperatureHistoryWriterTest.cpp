#include "XmlReaderWriterTest.h"
#include "Samples/TemperatureHistorySamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryWriterTest, save) {
	for (const auto& temperatureHistorySample : DtoReaderWriterTestSamples::TemperatureHistorySampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureHistorySample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = temperatureHistorySample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
